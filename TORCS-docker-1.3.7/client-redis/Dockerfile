FROM ubuntu:20.04

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    g++ \
    git-all \
    sudo \
    xorg openbox \
    xterm \
    make

RUN apt-get install -y alsa-utils

WORKDIR /tmp

ARG tf=tf

RUN git clone --recursive https://github.com/DrSgre/TORCS_multi_docker

WORKDIR TORCS_multi_docker/scr-client-cpp

RUN apt-get install -y \
    libboost-all-dev \
    libssl-dev \
    libgrpc-dev \
    libgrpc++-dev \
    libprotobuf-dev \
    protobuf-compiler-grpc

RUN apt-get install -y \
    wget \
    unzip \
    cmake

WORKDIR /tmp

RUN git clone https://github.com/redis/hiredis.git

WORKDIR ./hiredis

RUN make

RUN make install

WORKDIR /tmp

RUN git clone https://github.com/sewenew/redis-plus-plus.git

WORKDIR ./redis-plus-plus

RUN mkdir build

WORKDIR ./build

RUN cmake -DREDIS_PLUS_PLUS_CXX_STANDARD=11 ..

RUN make

RUN make install

WORKDIR /tmp/TORCS_multi_docker/scr-client-cpp-redis

RUN apt-get install -y libcpprest-dev

RUN make clean && make

CMD ./client host:172.20.0.3 port:3001

