<?php

declare(strict_types=1);

namespace ArangoDbDriverTest;

use ArangoDb\Connection;
use ArangoDb\Response;
use ArangoDb\Vpack;

final class TestUtil
{
    public static function getConnection(): Connection
    {
        $connection = new Connection(self::getConnectionParams());
        $connection->connect();
        return $connection;
    }

    public static function getDatabaseName(): string
    {
        if (! self::hasRequiredConnectionParams()) {
            throw new \RuntimeException('No connection params given');
        }

        return $GLOBALS['arangodb_dbname'];
    }

    public static function getConnectionParams(): array
    {
        if (! self::hasRequiredConnectionParams()) {
            throw new \RuntimeException('No connection params given');
        }

        return self::getSpecifiedConnectionParams();
    }

    public static function deleteCollection(Connection $connection, string $collection): void
    {
        try {
            $connection->delete('/_api/collection/' . $collection, Vpack::fromArray([]));
        } catch (\Throwable $e) {
            // needed if test deletes collection
        }
    }

    public static function wasSuccessful(Response $response): bool
    {
        $statusCode = $response->getHttpCode();

        return $statusCode === 201 || $statusCode === 200;
    }

    private static function hasRequiredConnectionParams(): bool
    {
        $env = getenv();

        return isset(
            $env['arangodb_username'],
            $env['arangodb_password'],
            $env['arangodb_host'],
            $env['arangodb_dbname']
        );
    }

    private static function getSpecifiedConnectionParams(): array
    {
        return [
            Connection::HOST => getenv('arangodb_host'),
            Connection::MAX_CHUNK_SIZE => 64,
            Connection::VST_VERSION => Connection::VST_VERSION_11,
        ];
    }
}
