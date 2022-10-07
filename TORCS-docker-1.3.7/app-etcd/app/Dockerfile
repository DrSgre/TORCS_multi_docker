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

RUN wget -O etcd-cpp-apiv3.zip https://github.com/etcd-cpp-apiv3/etcd-cpp-apiv3/archive/master.zip

RUN unzip etcd-cpp-apiv3.zip

WORKDIR /tmp/etcd-cpp-apiv3-master

RUN mkdir build

WORKDIR /tmp/etcd-cpp-apiv3-master/build

RUN cmake ..

RUN make -j$(nproc) && make install

WORKDIR /tmp

ARG tkh=tkh

RUN wget -O TORCS_multi_docker.zip https://github.com/DrSgre/TORCS_multi_docker/archive/master.zip

RUN unzip TORCS_multi_docker.zip

WORKDIR /tmp/TORCS_multi_docker-master/torcs-1.3.7-patched

RUN ln -s /usr/include/opencv4/opencv2 /usr/include/opencv2

RUN export CFLAGS="-fPIC" && export CPPFLAGS=$CFLAGS && export CXXFLAGS=$CFLAGS

RUN ./configure --prefix=$(pwd)/BUILD 

RUN make && make install && make datainstall

RUN ["chmod", "755", "./BUILD/bin/torcs"]

ARG LD_LIBRARY_PATH=/usr/local/lib/libetcd-cpp-api.so

RUN apt-get install iproute2 iputils-ping -y

CMD ./BUILD/bin/torcs

