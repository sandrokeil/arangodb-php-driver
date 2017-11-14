# ArangoDB PHP Driver 
Native PHP extension for [ArangoDB](https://arangodb.com/).

## Development

First install dependencies with

```
$ ./get_fuerte
```

and PHP dependencies with

```
$ docker run --rm -it --volume $(pwd):/app prooph/composer:7.1 update
```

This extension is build with [PHP-CPP](http://www.php-cpp.com/).
The build is simple with [Docker Compose](https://docs.docker.com/compose/install/).

```
$ docker-compose build arangodb-builder
$ docker-compose up -d --no-recreate arangodb
```

### Test

```
$ docker-compose run --rm arangodb-builder
$ docker-compose build php
$ docker-compose run --rm php vendor/bin/phpunit
```

Cleanup

```
$ docker-compose down -v
```

### Debugging
Check which symbols are in `build/libarangodb.so`. See [nm docs](https://linux.die.net/man/1/nm).

```
$ nm -g build/libarangodb.so | grep ArangoDb
$ ldd build/libarangodb.so
```

### UI
Open [management UI](http://localhost:8529/). You should see one collection
named `testobi`.
