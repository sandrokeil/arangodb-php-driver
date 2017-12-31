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
    autoconf \
    ' \
    && apk add --update $buildDeps \
    && docker-php-source extract

COPY deps /tmp

RUN cd /tmp/velocypack \
    && mkdir -p build \
    && cd build \
    && cmake .. -DCMAKE_CXX_FLAGS=-fPIC \
    && make install

WORKDIR /app
VOLUME ["/app"]

RUN apk add --update bash && rm -rf /tmp/*

RUN docker-php-source extract

ENTRYPOINT []
CMD bash -c "cd /app && phpize && ./configure --enable-arangodb && make && make install"
