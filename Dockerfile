FROM arangodb-php-driver-builder

COPY arangodb.ini /etc/php/7.1/cli/conf.d/
COPY arangodb.so /tmp

RUN mv /tmp/arangodb.so $(php-config --extension-dir)
