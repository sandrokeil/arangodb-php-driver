FROM sandrokeil/phpcpp

RUN apt-get update && apt-get install -y \
    libc-dev \
    libpcre3-dev

