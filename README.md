# TORCS_multi_docker
This repository contains the source code of a project aimed toward the modularization of the TORCS racing game simulator. 
* *torcs-1.3.7-patched*: contains the patched version of TORCS with scr_server and screenpipe to stream the game image.
* *scr-client-cpp*: contains the scr client used to connect and play TORCS in "Quick game" or "Practice" modes.
* *TORCS-docker-1.3.7*: contains the docker-compose.yml and Dockerfile of the 4 containers used for the project (app-etcd, watcher, client). 
<br/>
Representation of the current state of the modularization:
![Thesis work related notes](https://user-images.githubusercontent.com/16836365/188315940-67621799-5587-4dee-bba0-9c82324f8d86.jpg)
