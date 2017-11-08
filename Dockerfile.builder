FROM php:7.1-alpine

RUN buildDeps=' \
    bash \
    git \
    g++ \
    make \
    cmake \
    bison \
    flex \
    libstdc++ \
    libssl1.0 \
    boost-dev \
    curl-dev \
    openssl \
    openssl-dev \
    ' \
    && apk add --update $buildDeps

RUN docker-php-source extract

RUN apk add --update libc-dev pcre-dev

COPY deps /tmp

RUN cd /tmp/phpcpp \
        && sed -i 's/`\${PHP_CONFIG} \-\-ldflags`//g' Makefile \
        && make \
        && make install

#RUN cd /tmp/velocypack/build \
#        && cmake .. -DCMAKE_INSTALL_PREFIX=/usr -DBuildVelocyPackExamples=OFF -DCMAKE_CXX_FLAGS=-fPIC \
#        && make install

#RUN cd /tmp/fuerte \
#        && mkdir builddir \
#        && cd builddir \
#        && cmake .. -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_CXX_FLAGS=-fPIC -DCMAKE_BUILD_TYPE=Release \
#        && make install

WORKDIR /app
VOLUME ["/app"]

RUN apk add --update bash && rm -rf /tmp/*

ENTRYPOINT []
CMD bash -c "cd /app/build && cmake .. -DPHPCPP_ARCH=x86_64 -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS=-fPIC && make"
