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

RUN git clone --recursive https://github.com/DrSgre/TORCS_multi_docker

WORKDIR TORCS_multi_docker/scr-client-cpp

RUN make clean && make

CMD ./client host:172.20.0.3 port:3001

