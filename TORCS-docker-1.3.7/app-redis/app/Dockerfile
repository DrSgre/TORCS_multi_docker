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

ARG tvd=tvd

RUN wget -O TORCS_multi_docker.zip https://github.com/DrSgre/TORCS_multi_docker/archive/master.zip

RUN unzip TORCS_multi_docker.zip

WORKDIR /tmp/TORCS_multi_docker-master/torcs-1.3.7-patched-redis

RUN ln -s /usr/include/opencv4/opencv2 /usr/include/opencv2

RUN export CFLAGS="-fPIC" && export CPPFLAGS=$CFLAGS && export CXXFLAGS=$CFLAGS

RUN ./configure --prefix=$(pwd)/BUILD 

RUN make && make install && make datainstall

RUN ["chmod", "755", "./BUILD/bin/torcs"]

RUN apt-get install iproute2 iputils-ping -y

CMD ./BUILD/bin/torcs

