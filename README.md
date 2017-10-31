# ArangoDB PHP Driver 
Native PHP extension for [ArangoDB](https://arangodb.com/).

## Development

This extension is build with [PHP-CPP](http://www.php-cpp.com/).
The build is simple with [Docker Compose](https://docs.docker.com/compose/install/).

To build the c archive from go driver run `docker run --rm -it -v $(pwd)/go:/go golang:1.9.1-stretch make build`

```
$ cd go
$ ln -s arangodb.a libarangodb.a
$ cd ..
```

```
$ docker-compose run --rm arangodb-php-driver-builder
$ docker-compose up -d --no-recreate
```

### Test

```
$ docker-compose run --rm arangodb-php-driver-builder
$ docker-compose build phptester
$ docker-compose run --rm phptester php test.php
```

Cleanup

```
$ docker-compose down -v
```

### UI
Open [management UI](http://localhost:8529/). You should see one collection
named `testobi`.
