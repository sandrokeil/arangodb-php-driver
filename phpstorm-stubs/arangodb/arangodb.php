<?php
/**
 * ArangoDb Extension Stub File
 * @version 0.1.0
 * Documentation taken from https://github.com/sandrokeil/arangodb-php-driver
 *
 * Add it to PHPStorm via
 *
 * 1. git clone https://github.com/sandrokeil/arangodb-php-driver
 * 2. Open Preferences -> Languages & Frameworks -> PHP
 * 3. Click PHP Runtime tab
 * 4. Click Advanced Settings at the bottom
 * 5. Next to Default stubs path, click the ... and navigate to the root of your arangodb-php-driver/phpstorm-stubs folder
 */

/**
 * Unlike the official ArangoDB PHP library, this C++ PHP driver uses fuerte and valocypack.
 *
 * @link https://github.com/sandrokeil/arangodb-php-driver
 */

namespace ArangoDb {

    /**
     * Handles connection to ArangoDb
     *
     * @link https://github.com/sandrokeil/arangodb-php-driver/blob/master/src/connection.h
     */
    final class Connection
    {
        public CONST HOST = 'host';
        public CONST USER = 'user';
        public CONST PASSWORD = 'password';
        public CONST MAX_CHUNK_SIZE = 'max_chunk_size';
        public CONST VST_VERSION = 'vst_version';
        public CONST VST_VERSION_10 = 0;
        public CONST VST_VERSION_11 = 1;

        /**
         * @param array $options Connection options
         * @return Connection
         */
        public function __construct(array $options)
        {
        }

        /**
         * Sets the thread count for the loop service
         *
         * @param int $threadCount Number of threads
         * @return void
         */
        public function setThreadCount(int $threadCount): void
        {
        }

        /**
         * Establishes a connection to ArangoDb
         */
        public function connect(): void
        {
        }

        /**
         * Sends a HTTP DELETE request
         *
         * @param string $path URI path of ArangoDB Rest API
         * @param Vpack $vpack Request data
         * @param array $queryParams Map of query params, added to URI path
         */
        public function delete(string $path, Vpack $vpack, array $queryParams = null): Response
        {
        }

        /**
         * Sends a HTTP GET request
         *
         * @param string $path URI path of ArangoDB Rest API
         * @param Vpack $vpack Request data
         * @param array|null $queryParams Map of query params added to URI path
         */
        public function get(string $path, Vpack $vpack, array $queryParams = null): Response
        {
        }

        /**
         * Sends a HTTP POST request
         *
         * @param string $path URI path of ArangoDB Rest API
         * @param Vpack $vpack Request data
         * @param array|null $queryParams Map of query params added to URI path
         */
        public function post(string $path, Vpack $vpack, array $queryParams = null): Response
        {
        }

        /**
         * Sends a HTTP PUT request
         *
         * @param string $path URI path of ArangoDB Rest API
         * @param Vpack $vpack Request data
         * @param array|null $queryParams Map of query params added to URI path
         */
        public function put(string $path, Vpack $vpack, array $queryParams = null): Response
        {
        }

        /**
         * Sends a HTTP HEAD request
         *
         * @param string $path URI path of ArangoDB Rest API
         * @param Vpack $vpack Request data
         * @param array|null $queryParams Map of query params added to URI path
         */
        public function head(string $path, Vpack $vpack, array $queryParams = null): Response
        {
        }

        /**
         * Sends a HTTP PATCH request
         *
         * @param string $path URI path of ArangoDB Rest API
         * @param Vpack $vpack Request data
         * @param array|null $queryParams Map of query params added to URI path
         */
        public function patch(string $path, Vpack $vpack, array $queryParams = null): Response
        {
        }

        /**
         * Sends a HTTP OPTIONS request
         *
         * @param string $path URI path of ArangoDB Rest API
         * @param Vpack $vpack Request data
         * @param array|null $queryParams Map of query params added to URI path
         */
        public function options(string $path, Vpack $vpack, array $queryParams = null): Response
        {
        }

        /**
         * Sends a query
         *
         * @param Vpack $vpack
         * @return Cursor
         */
        public function query(Vpack $vpack): Cursor
        {

        }
    }

    /**
     * Handles responses
     *
     * @link https://github.com/sandrokeil/arangodb-php-driver/blob/master/src/response.h
     */
    final class Response
    {
        /**
         * @return int HTTP status code
         */
        public function getHttpCode(): int
        {
        }

        /**
         * @return string Response body
         */
        public function getBody(): string
        {
        }
    }

    /**
     * Handles requests
     *
     * @link https://github.com/sandrokeil/arangodb-php-driver/blob/master/src/request.h
     */
    final class Request
    {
        public CONST METHOD_DELETE = 0;
        public CONST METHOD_GET = 1;
        public CONST METHOD_POST = 2;
        public CONST METHOD_PUT = 3;
        public CONST METHOD_HEAD = 4;
        public CONST METHOD_PATCH = 5;
        public CONST METHOD_OPTIONS = 6;

        /**
         * Create Request
         *
         * @param string $path URI path of ArangoDB Rest API
         * @param Vpack $vpack Request data
         * @param array|null $queryParams Map of query params added to URI path
         *
         * @return Request
         */
        public function __construct(string $path, Vpack $vpack, array $queryParams = null)
        {

        }
    }

    /**
     * Handles velocypack implementation
     *
     * Super fast and efficient access of JSON data.
     *
     * @link https://github.com/arangodb/velocypack
     */
    final class Vpack
    {
        /**
         * Creates a Vpack instance from array data
         *
         * @param array $data
         * @return Vpack
         */
        public static function fromArray(array $data): Vpack
        {
        }

        /**
         * Creates a Vpack instance from JSON data
         *
         * @param string $json JSON data
         * @return Vpack
         */
        public static function fromJson(string $json): Vpack
        {
        }
    }

    /**
     * Handles queries
     *
     * @link https://github.com/sandrokeil/arangodb-php-driver/blob/master/src/cursorIterator.h
     */
    final class Cursor implements \Iterator
    {
        /**
         * Return the current element
         * @link http://php.net/manual/en/iterator.current.php
         * @return mixed Can return any type.
         */
        public function current()
        {
        }

        /**
         * Move forward to next element
         * @link http://php.net/manual/en/iterator.next.php
         * @return void Any returned value is ignored.
         */
        public function next()
        {
        }

        /**
         * Return the key of the current element
         * @link http://php.net/manual/en/iterator.key.php
         * @return mixed scalar on success, or null on failure.
         */
        public function key()
        {
        }

        /**
         * Checks if current position is valid
         * @link http://php.net/manual/en/iterator.valid.php
         * @return boolean The return value will be casted to boolean and then evaluated.
         * Returns true on success or false on failure.
         */
        public function valid()
        {
        }

        /**
         * Rewind the Iterator to the first element
         *
         * If iteration has already begun, this will throw an exception.
         *
         * @link http://php.net/manual/en/iterator.rewind.php
         * @return void Any returned value is ignored.
         */
        public function rewind()
        {
        }
    }
}
