FROM ubuntu:20.04

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    g++ \
    git-all \
    wget \
    unzip \
    sudo \
    cmake \
    make \
    sudo \
    libgtk2.0-dev \
    pkg-config

RUN apt-get install ffmpeg libsm6 libxext6  -y

RUN apt-get install -y libcpprest-dev

RUN apt-get install -y libboost-all-dev libssl-dev

RUN apt-get install -y libopencv-dev python3-opencv

RUN apt-get install -y libgrpc-dev \
        libgrpc++-dev \
        libprotobuf-dev \
        protobuf-compiler-grpc \
        g++

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

WORKDIR /tmp

ARG ai=ai

RUN wget -O TORCS_multi_docker.zip https://github.com/DrSgre/TORCS_multi_docker/archive/master.zip

RUN unzip TORCS_multi_docker.zip

WORKDIR /tmp/TORCS_multi_docker-master/torcs-1.3.7-patched-redis/new_screenpipe

RUN ["chmod", "755", "./screenpipe_client"]

RUN export NO_AT_BRIDGE=1

CMD tail -f /dev/null

