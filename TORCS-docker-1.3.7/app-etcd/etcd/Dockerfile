FROM ubuntu:20.04

ARG DEBIAN_FRONTEND=noninteractive
ARG DISPLAY=$DISPLAY

RUN apt-get update && apt-get install -y \
    sudo \
    cmake \
    wget \
    unzip \
    make

RUN sudo apt search golang-go && sudo apt search gccgo-go

RUN sudo apt install golang-go -y

WORKDIR /tmp

RUN wget -O etcd-cpp-apiv3.zip https://github.com/etcd-cpp-apiv3/etcd-cpp-apiv3/archive/master.zip

RUN unzip etcd-cpp-apiv3.zip

WORKDIR /tmp/etcd-cpp-apiv3-master

RUN mkdir build

WORKDIR /tmp/etcd-cpp-apiv3-master/build

RUN cmake ..

RUN make -j$(nproc) && make install

WORKDIR /tmp

RUN wget -O TORCS_multi_docker.zip https://github.com/DrSgre/TORCS_multi_docker/archive/master.zip

RUN unzip TORCS_multi_docker.zip

WORKDIR /tmp/TORCS_multi_docker-master/etcd/

RUN sudo ./build.sh

RUN export PATH="$PATH:`pwd`/bin"

CMD ./etcd

