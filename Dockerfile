FROM arangodb-builder

COPY docker-entrypoint.sh /usr/local/bin/

RUN echo "extension=libarangodb.so" > /usr/local/etc/php/conf.d/arangodb.ini \
    && ln -s usr/local/bin/docker-entrypoint.sh /entrypoint.sh # backwards compat

ENTRYPOINT ["docker-entrypoint.sh"]
