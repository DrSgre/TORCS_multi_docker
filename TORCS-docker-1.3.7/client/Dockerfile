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

RUN wget -O etcd-cpp-apiv3.zip https://github.com/etcd-cpp-apiv3/etcd-cpp-apiv3/archive/master.zip

RUN unzip etcd-cpp-apiv3.zip

RUN apt-get install -y libcpprest-dev

WORKDIR /tmp/etcd-cpp-apiv3-master

RUN mkdir build

WORKDIR /tmp/etcd-cpp-apiv3-master/build

RUN cmake ..

RUN make -j$(nproc) && make install  

WORKDIR /tmp/TORCS_multi_docker/scr-client-cpp

RUN make clean && make

ARG LD_LIBRARY_PATH=/usr/local/lib/libetcd-cpp-api.so

RUN ldconfig

CMD ./client host:172.20.0.6 port:3001

