<?php

declare(strict_types=1);

namespace ArangoDbDriverTest;

use ArangoDb\Type\InsertDocument;

use PHPUnit\Framework\TestCase;

/**
 * @group type
 */
class InsertDocumentTest extends TestCase
{
    public function it_can_be_created_without_options()
    {
        $cut = InsertDocument:::with('myCollectionName', [['_id' => 1, 'test' => 2]]);
        $this->assertInstanceOf(\ArangoDb\InsertDocument::class, $cut);
    }
}
