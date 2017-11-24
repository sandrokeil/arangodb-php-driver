<?php

declare(strict_types=1);

namespace ArangoDbDriverTest;

use ArangoDb\Connection;
use ArangoDb\Response;
use ArangoDb\Vpack;

final class TestUtil
{
    public static function getConnection(bool $connect = true): Connection
    {
        $connection = new Connection(self::getConnectionParams());
        if ($connect) {
            $connection->connect();
        }
        return $connection;
    }

    public static function getDatabaseName(): string
    {
        if (!self::hasRequiredConnectionParams()) {
            throw new \RuntimeException('No connection params given');
        }

        return $GLOBALS['arangodb_dbname'];
    }

    public static function getConnectionParams(): array
    {
        if (!self::hasRequiredConnectionParams()) {
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

        return $statusCode >= 200 && $statusCode <= 299;
    }

    public static function getVpackCreateCollection(string $collectionName): Vpack
    {
        return Vpack::fromArray([
            'name' => $collectionName,
            'keyOptions' => [
                'allowUserKeys' => false,
                'type' => 'autoincrement',
                'increment' => 1,
                'offset' => 1,
            ],
        ]);
    }

    public static function getVpackCreateIndex(): Vpack
    {
        return Vpack::fromArray([
            'type' => 'hash',
            'fields' => [
                'real_stream_name',
            ],
            'selectivityEstimate' => 1,
            'unique' => true,
            'sparse' => false,
        ]);
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
