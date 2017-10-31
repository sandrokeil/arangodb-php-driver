# ArangoDB PHP Driver 
Native PHP extension for [ArangoDB](https://arangodb.com/).

## Development

First install dependencies with

```
$ ./get_fuerte
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
$ docker-compose run --rm phptester php test.php
```

Cleanup

```
$ docker-compose down -v
```

### UI
Open [management UI](http://localhost:8529/). You should see one collection
named `testobi`.
