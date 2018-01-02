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
    && cmake .. -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS=-fPIC \
    && make install

RUN cd /tmp/fuerte \
    && cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS=-fPIC \
    && make install

WORKDIR /app
VOLUME ["/app"]

RUN apk add --update bash && rm -rf /tmp/*

ENTRYPOINT []
CMD bash -c "cd /app && phpize && ./configure && make all -j4"
