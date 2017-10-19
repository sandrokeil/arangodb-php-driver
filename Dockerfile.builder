FROM sandrokeil/phpcpp

COPY deps /tmp

RUN cd /tmp/velocypack && \
    mkdir -p build && \
    cd build && \
    cmake .. && \
    make install
