#include "cursor.h"

namespace arangodb { namespace fuerte { namespace php {

    Cursor::Cursor(Connection* connection, Vpack* vpack): vpack(vpack), connection(connection)
    {
        this->loadFirstBatch();
    }

    void Cursor::loadFirstBatch()
    {
        Request request("/_api/cursor", this->vpack);
        request.setHttpMethod(Request::METHOD_POST);

        Response* response = this->connection->sendRequest(&request);
        this->response = response;

        if(this->response->getFuerteResponse()->slices().front().get("error").getBool()) {
            throw Php::Exception(
                "Error while executing query: " +
                this->response->getFuerteResponse()->slices().front().get("errorMessage").copyString()
            );
        }

        this->hasMore = this->response->getFuerteResponse()->slices().front().get("hasMore").getBool();
        this->batchSize = this->response->getFuerteResponse()->slices().front().get("result").length();
        this->number = this->batchSize;

        if (this->response->getFuerteResponse()->slices().front().hasKey("count")) {
            this->number = this->response->getFuerteResponse()->slices().front().get("count").getInt();
        }

        if(this->hasMore) {
            this->id = this->response->getFuerteResponse()->slices().front().get("id").copyString();
        }
    }

    void Cursor::loadMore()
    {
        Vpack emptyVpack;

        Request request("/_api/cursor/" + this->id, &emptyVpack);
        request.setHttpMethod(Request::METHOD_PUT);

        Response* response = this->connection->sendRequest(&request);

        this->response = response;
        this->hasMore = this->response->getFuerteResponse()->slices().front().get("hasMore").getBool();
        this->batchSize = this->response->getFuerteResponse()->slices().front().get("result").length();
        this->number = this->batchSize;

        if (this->response->getFuerteResponse()->slices().front().hasKey("count")) {
            this->number = this->response->getFuerteResponse()->slices().front().get("count").getInt();
        }
    }

    Php::Value Cursor::valid()
    {
        if(this->batchSize > this->position) {
            return true;
        } else if(this->hasMore) {
            this->loadMore();
            this->position = 0;
            return true;
        }

        return false;
    }

    Php::Value Cursor::current()
    {
        std::string body;

        try {
            vp::Slice slice = this->response->getFuerteResponse()->slices().front().get("result").at(this->position);
            vp::Options dumperOptions;

            vp::StringSink sink(&body);
            vp::Dumper dumper(&sink, &dumperOptions);
            dumper.dump(slice);
        } catch(vp::Exception const& e) {
            throw Php::Exception(e.what());
        }

        return body;
    }

    Php::Value Cursor::key()
    {
        return this->position;
    }

    void Cursor::next()
    {
        this->position++;
    }

    void Cursor::rewind()
    {
    }

    long Cursor::count() {
        return static_cast<int>(this->number);
    }

    Php::Value Cursor::getCount() {
        return static_cast<int>(this->number);
    }

}}}
