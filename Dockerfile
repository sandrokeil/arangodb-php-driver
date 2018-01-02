FROM arangodb-builder

COPY modules/arangodb.so /tmp

RUN echo "extension=arangodb.so" > /usr/local/etc/php/conf.d/arangodb.ini \
    && mv /tmp/arangodb.so $(php-config --extension-dir)
