FROM ubuntu:20.04

ARG DEBIAN_FRONTEND=noninteractive
ARG DISPLAY=$DISPLAY

RUN apt-get update && apt-get install -y \
    git-all \
    sudo \
    cmake \
    wget \
    unzip \
    make

RUN sudo apt-get install software-properties-common -y

RUN sudo apt update

RUN sudo add-apt-repository ppa:longsleep/golang-backports

RUN sudo apt update

RUN sudo apt install golang -y

WORKDIR /tmp

ARG test=test

RUN wget -O TORCS_multi_docker.zip https://github.com/DrSgre/TORCS_multi_docker/archive/master.zip

RUN unzip TORCS_multi_docker.zip

WORKDIR /tmp/TORCS_multi_docker-master/etcd/

RUN go version

RUN git init

RUN sudo ./build.sh

RUN export PATH="$PATH:`pwd`/bin"

RUN ["chmod", "777", "./run_w_defrag.sh"]

CMD ./run_w_defrag.sh

