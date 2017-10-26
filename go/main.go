package main

import (
    "C"

    "github.com/arangodb/go-velocypack"
)

// main is required to build a shared library, but does nothing
func main() {}

func test() {
    var b velocypack.Builder

    b.Bytes()

    b.Close()
}

