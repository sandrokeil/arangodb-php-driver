# ArangoDB PHP Driver 
Native PHP extension for [ArangoDB](https://arangodb.com/).

## Development

First install dependencies with

```
$ ./get_fuerte
```

This extension is build with [PHP-CPP](http://www.php-cpp.com/) . Build the extension with Docker:

```
$ docker run --rm -v $(pwd):/app sandrokeil/phpcpp /bin/bash -c "cd build; cmake -DCMAKE_BUILD_TYPE=Release -DBuildExamples=ON -DBuildTests=ON -DBuildTools=ON .. ; make"
$ docker build -t arangodb-php-driver-builder -f Dockerfile.builder .
$ docker run --rm -v $(pwd):/app arangodb-php-driver-builder make && sudo chown skeil:skeil . -R
$ docker build -t arangodb-php-driver-test .
$ docker run --rm -it -v $(pwd):/app arangodb-php-driver-test php test.php
```

```
rm ./src/main.o && rm arangodb.so && docker run --rm -v $(pwd):/app arangodb-php-driver-builder make && sudo chown skeil:skeil . -R && docker build -t arangodb-php-driver-test . && docker run --rm -it -v $(pwd):/app arangodb-php-driver-test php test.php
```

### Test

```
$ docker build -t arangodb-php-driver-test .
$ docker run --rm -it -v $(pwd):/app arangodb-php-driver-test php test.php
```
