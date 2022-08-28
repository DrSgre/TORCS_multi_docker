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
    git-all \
    sudo \
    xorg openbox \
    xterm \
    make

RUN apt-get install -y alsa-utils

WORKDIR /tmp

RUN git clone --recursive https://github.com/DrSgre/TORCS_multi_docker

RUN ls

WORKDIR ./TORCS_multi_docker

RUN ls

WORKDIR ./torcs-1.3.7-patched

RUN export CFLAGS="-fPIC" && export CPPFLAGS=$CFLAGS && export CXXFLAGS=$CFLAGS

RUN ls

RUN ./configure --prefix=$(pwd)/BUILD

RUN make && make install && make datainstall

RUN ["chmod", "755", "./BUILD/bin/torcs"]

WORKDIR /tmp/torcs-1.3.7-patched/BUILD/bin/

CMD sudo ./torcs


## DISCARDED DISPLAY MANAGEMENT

#ENV DISPLAY=':0.0'
#RUN sed -i 's/ForwardX11 no/ForwardX11 yes/' /etc/ssh/ssh_config
#RUN cat /etc/ssh/ssh_config
#RUN touch ~/.Xauthority
#RUN xauth generate $DISPLAY . trusted 
#RUN xauth add ${HOST}:0 . $(xxd -l 16 -p /dev/urandom)
#RUN xauth list
#RUN xhost +local:docker 
#RUN xterm
#CMD sudo ./torcs -r ~/.torcs/config/raceman/champ.xml
#CMD sudo ./torcs
#ENTRYPOINT ["tail", "-f", "/dev/null"]

## DISCARDED SSH MANAGEMENT

#RUN touch /tmp/myfolder
#RUN apt-get update && apt-get install -y openssh-server
#RUN mkdir /var/run/sshd
#RUN echo 'root:mypassword' | chpasswd
#RUN sed -i 's/#PermitRootLogin prohibit-password/PermitRootLogin yes/' /etc/ssh/sshd_config
#RUN sed 's@session\s*required\s*pam_loginuid.so@session optional pam_loginuid.so@g' -i /etc/pam.d/sshd
#EXPOSE 22
#RUN echo 'root:Drsgre.1' | chpasswd
#RUN mkdir -p /home/app/.ssh
#RUN echo "DISPLAY=$DISPLAY" >> /home/app/.ssh/environment
#WORKDIR /
#CMD ["/usr/sbin/sshd", "-D"]
