# ArangoDB PHP Driver 
Native PHP extension for [ArangoDB](https://arangodb.com/).

## Development

First install dependencies with

```
$ ./get_fuerte
```

This extension is build with [PHP-CPP](http://www.php-cpp.com/) . Build the extension with Docker:

```
$ docker build -t arangodb-php-driver-builder -f Dockerfile.builder .
$ docker run --rm -v $(pwd):/app arangodb-php-driver-builder make && sudo chown $(id -u -n):$(id -g -n) . -R . -R
$ docker build -t arangodb-php-driver-test .
$ docker run --rm -it -v $(pwd):/app arangodb-php-driver-test php test.php
```

One line to rebuild the extension:

```
rm -rf ./src/main.o && rm -rf arangodb.so && docker run --rm -v $(pwd):/app arangodb-php-driver-builder make && sudo chown $(id -u -n):$(id -g -n) . -R . -R && docker build -t arangodb-php-driver-test . && docker run --rm -it -v $(pwd):/app arangodb-php-driver-test php test.php
```

### Test

```
$ docker build -t arangodb-php-driver-test .
$ docker run --rm -it -v $(pwd):/app arangodb-php-driver-test php test.php
```
