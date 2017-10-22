FROM sandrokeil/phpcpp

COPY deps /tmp

RUN cd /tmp/velocypack/build && \
    cmake .. -DCMAKE_CXX_FLAGS=-fPIC && \
    make install

RUN apt-get update && apt-get install -y \
    libc-dev \
    libpcre3-dev

RUN cd /tmp/fuerte && \
    mkdir buildir && \
    cd buildir && \
    cmake .. -DCMAKE_CXX_FLAGS=-fPIC -DCMAKE_BUILD_TYPE=Release && \
    make install
