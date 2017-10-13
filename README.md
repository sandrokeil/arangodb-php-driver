# ArangoDB PHP Driver 
Native PHP extension for [ArangoDB](https://arangodb.com/).

## Development

First install dependencies with

```
$ ./get_fuerte
```

This extension is build with [PHP-CPP](http://www.php-cpp.com/) . Build the extension with Docker:

```
$ docker run --rm -v $(pwd):/app sandrokeil/phpcpp make
```
