FROM ubuntu:20.04

ARG DEBIAN_FRONTEND=noninteractive
ARG DISPLAY=$DISPLAY

RUN apt-get update && apt-get install -y \
    g++ \
    libglib2.0-dev \
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    freeglut3-dev \
    libplib-dev \
    libopenal-dev \
    libalut-dev \
    libxi-dev \
    libxmu-dev \
    libxrender-dev \
    libxrandr-dev \
    libpng-dev \
    libvorbis-dev \
    libxxf86vm-dev \
    wget \
    unzip \
    sudo \
    xorg openbox \
    xterm \
    cmake \
    make

RUN apt-get install -y alsa-utils

RUN apt-get install -y libopencv-dev python3-opencv

RUN apt-get install -y libcpprest-dev

RUN apt-get install -y libboost-all-dev libssl-dev

RUN apt-get install -y libgrpc-dev \
        libgrpc++-dev \
        libprotobuf-dev \
        protobuf-compiler-grpc

WORKDIR /tmp

RUN apt-get install -y git-all

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

ARG tvg=tvg

RUN wget -O TORCS_multi_docker.zip https://github.com/DrSgre/TORCS_multi_docker/archive/cluster_tests.zip

RUN unzip TORCS_multi_docker.zip

WORKDIR /tmp/TORCS_multi_docker-cluster_tests/torcs-1.3.7-patched-redis

RUN ln -s /usr/include/opencv4/opencv2 /usr/include/opencv2

RUN export CFLAGS="-fPIC" && export CPPFLAGS=$CFLAGS && export CXXFLAGS=$CFLAGS

RUN ./configure --prefix=$(pwd)/BUILD 

RUN make && make install && make datainstall

RUN ["chmod", "755", "./BUILD/bin/torcs"]

RUN apt-get install iproute2 iputils-ping -y

RUN sudo apt install lsb-release -y

RUN sudo apt install curl -y

RUN sudo apt install gpg -y

RUN curl -fsSL https://packages.redis.io/gpg | sudo gpg --dearmor -o /usr/share/keyrings/redis-archive-keyring.gpg

RUN sudo apt-get update && sudo apt-get install redis -y

CMD redis-cli -h 172.20.0.2 -p 6379 --cluster fix 172.20.0.2:6379 && ./BUILD/bin/torcs

