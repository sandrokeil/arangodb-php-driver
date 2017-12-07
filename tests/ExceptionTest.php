<?php

declare(strict_types=1);

namespace ArangoDbDriverTest;

use PHPUnit\Framework\TestCase;
use ArangoDb\Connection;

/**
 * @group connection
 */
class ExceptionTest extends TestCase
{
    /**
     * @test
     */
    public function it_throws_valid_custom_exception(): void
    {
        try {
            $connection = new Connection([
                'unknown' => 'error'
            ]);

            $connection->connect();

            $this->assertTrue(false);

        } catch(\Throwable $e) {
            $this->assertInstanceOf(\Exception::class, $e);
            $this->assertInstanceOf(\ArangoDb\Exception::class, $e);
            $this->assertInstanceOf(\ArangoDb\RuntimeException::class, $e);
            $this->assertInstanceOf(\ArangoDb\InvalidOptionException::class, $e);
        }
    }
}
