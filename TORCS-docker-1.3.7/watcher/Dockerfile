FROM ubuntu:20.04

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    wget \
    unzip \
    sudo \
    pip \
    libgtk2.0-dev \
    pkg-config

RUN apt-get install ffmpeg libsm6 libxext6  -y

RUN pip install numpy && pip install etcd3 && pip install opencv-python && pip install opencv-python-headless && pip install protobuf==3.20.*

RUN pip install opencv-contrib-python

WORKDIR /tmp

RUN wget -O TORCS_multi_docker.zip https://github.com/DrSgre/TORCS_multi_docker/archive/master.zip

RUN unzip TORCS_multi_docker.zip

WORKDIR /tmp/TORCS_multi_docker-master/torcs-1.3.7-patched/screenpipe

CMD sudo python3 ./screenpipe_client.py

