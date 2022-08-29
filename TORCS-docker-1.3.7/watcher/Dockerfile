FROM ubuntu:20.04

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    git-all \
    sudo

RUN sudo apt install python3.8

WORKDIR /tmp

RUN git clone --recursive https://github.com/DrSgre/TORCS_multi_docker

WORKDIR TORCS_multi_docker/torcs-1.3.7-patched/screenpipe

CMD sudo python3 ./screenpipe_client.py

