#include "cursor.h"
#include "cursorIterator.h"

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
        this->hasMore = this->response->getFuerteResponse()->slices().front().get("hasMore").getBool();
        this->id = this->response->getFuerteResponse()->slices().front().get("id").copyString();
        this->batchSize = this->response->getFuerteResponse()->slices().front().get("result").length();
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
    }


    Php::Iterator* Cursor::getIterator()
    {
        return new CursorIterator(this);
    }

}}}
