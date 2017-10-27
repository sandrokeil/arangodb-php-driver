package main

import (
    "C"

    "fmt"
    "github.com/arangodb/go-velocypack"
)

// main is required to build a shared library, but does nothing
func main() {}

//export test
func test() {
    var b velocypack.Builder

    fmt.Println(b.Bytes())

    b.Close()
}

