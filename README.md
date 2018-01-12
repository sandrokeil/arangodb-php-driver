# ArangoDB PHP Driver 
Native PHP extension for [ArangoDB](https://arangodb.com/).

## Development

First install dependencies with

```
$ ./get_fuerte
```

This extension is build with [native Zend Engine](http://www.phpinternalsbook.com/#php-7).
The build is simple with [Docker Compose](https://docs.docker.com/compose/install/).

```
$ docker-compose build arangodb-builder
$ docker-compose up -d --no-recreate arangodb
```

### Test

```
$ docker-compose run --rm arangodb-builder
$ docker-compose run --rm arangodb-builder make test
$ docker-compose build php
$ docker-compose run --rm php vendor/bin/phpunit
```

Cleanup

```
$ docker-compose down -v
$ docker-compose run --rm arangodb-builder make clean
$ docker-compose run --rm arangodb-builder phpize --clean
```

### Debugging
Check which symbols are in `modules/arangodb.so`. See [nm docs](https://linux.die.net/man/1/nm).

```
$ nm -g modules/arangodb.so | grep arangodb
$ ldd modules/arangodb.so
```

### UI
Open [management UI](http://localhost:8529/). You should see one collection
named `testobi`.
