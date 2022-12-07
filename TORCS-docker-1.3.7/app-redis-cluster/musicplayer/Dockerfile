FROM ubuntu:20.04

ARG DEBIAN_FRONTEND=noninteractive

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
    sudo 

RUN apt-get install -y alsa-utils

RUN apt-get install -y libopencv-dev python3-opencv

WORKDIR /tmp

ARG tg=tg

RUN wget -O TORCS_multi_docker.zip https://github.com/DrSgre/TORCS_multi_docker/archive/cluster_tests.zip

RUN unzip TORCS_multi_docker.zip

WORKDIR /tmp/TORCS_multi_docker-cluster_tests/musicplayer

RUN ["chmod", "777", "./main"]

CMD ./main

