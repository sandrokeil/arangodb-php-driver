FROM arangodb-php-driver-builder

RUN apt-get update && apt-get install -y \
    php-xdebug \
    php7.1-dom

COPY arangodb.ini /etc/php/7.1/cli/conf.d/
COPY arangodb.so /tmp

RUN mv /tmp/arangodb.so $(php-config --extension-dir)
