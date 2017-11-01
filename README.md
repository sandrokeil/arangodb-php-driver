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
$ docker-compose build
$ docker-compose up -d --no-recreate arangodb
```

### Test

```
$ docker-compose run --rm arangodb-php-driver-builder
$ docker-compose build phptester
$ docker-compose run --rm phptester vendor/bin/phpunit
```

or in one line

```
$ docker-compose run --rm arangodb-php-driver-builder && docker-compose build phptester && docker-compose run --rm phptester vendor/bin/phpunit
```

Cleanup

```
$ docker-compose down -v
```

### Debugging
Check which symbols are in `arangodb.so`

```
$ nm -g arangodb.so | grep ArangoDb
```

### UI
Open [management UI](http://localhost:8529/). You should see one collection
named `testobi`.
