FROM ubuntu:20.04

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
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

ARG t=t

RUN wget -O TORCS_multi_docker.zip https://github.com/DrSgre/TORCS_multi_docker/archive/master.zip

RUN unzip TORCS_multi_docker.zip

RUN wget -O etcd-cpp-apiv3.zip https://github.com/etcd-cpp-apiv3/etcd-cpp-apiv3/archive/master.zip

RUN unzip etcd-cpp-apiv3.zip

WORKDIR /tmp/etcd-cpp-apiv3-master

RUN mkdir build

WORKDIR /tmp/etcd-cpp-apiv3-master/build

RUN cmake ..

RUN make -j$(nproc) && make install

WORKDIR /tmp/TORCS_multi_docker-master/torcs-1.3.7-patched/new_screenpipe

RUN ["chmod", "755", "./screenpipe_client"]

RUN export NO_AT_BRIDGE=1

CMD tail -f /dev/null

