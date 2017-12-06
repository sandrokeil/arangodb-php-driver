FROM php:7.1-alpine

RUN buildDeps=' \
    bash \
    git \
    g++ \
    make \
    cmake \
    libstdc++ \
    libssl1.0 \
    boost-dev \
    curl-dev \
    openssl \
    openssl-dev \
    libc-dev \
    pcre-dev \
    ' \
    && apk add --update $buildDeps \
    && docker-php-source extract

COPY deps /tmp

RUN cd /tmp/phpcpp \
        && sed -i 's/`\${PHP_CONFIG} \-\-ldflags`//g' Makefile \
        && make \
        && make install

WORKDIR /app
VOLUME ["/app"]

RUN apk add --update bash && rm -rf /tmp/*

RUN docker-php-source extract

ENTRYPOINT []
CMD bash -c "cd /app/build && cmake .. -DPHPCPP_ARCH=x86_64 -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS=-fPIC && make"
