#include "InsertDocument.h"

namespace arangodb {
    namespace fuerte {
        namespace php {

            InsertDocument::InsertDocument(std::string collectionName, Php::Value streamEvents, Php::Value options) {
                this->collectionName = std::move(collectionName);
                this->streamEvents = std::move(streamEvents);
                this->options = std::move(options);
            }

            Php::Value InsertDocument::with(Php::Parameters &params) {
                if (!params[0].isString()) {
                    throw Php::Exception("$collectionName must be of type string");
                }

                if (!params[1].isArray()) {
                    throw Php::Exception("$eventStreams must be of type array");
                }

                return Php::Object("ArangoDb\\Type\\InsertDocument", new InsertDocument(params[0], params[1], params[2]));
            }

        }
    }
}
