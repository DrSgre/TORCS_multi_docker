# TORCS_multi_docker
## Repository structure
This repository contains the source code of a project aimed toward the modularization of the TORCS racing game simulator. The content of the repository is organized as follows:
* *Demo video*: contains the video demonstration of the thesis project.
* *Experments data*: contains the files and data related to the experiments conducted on the system.
* *TORCS-docker-1.3.7*: contains the docker-compose.yml and Dockerfile of the containers used for the project, both the ETCD and the Redis versions.
* *Thesis document*: contains the LaTeX project of the thesis document.
* *musicplayer*: contains the decoupled TORCS Music Player library.
* *scr-client-cpp-redis*: contains the Redis version of the scr client used to connect and play TORCS in "Quick game" or "Practice" modes.
* *scr-client-cpp*: contains the ETCD version of the scr client used to connect and play TORCS in "Quick game" or "Practice" modes.
* *torcs-1.3.7-patched-redis*: contains the patched version of TORCS with scr_server and screenpipe to stream the game image, based on Redis.
* *torcs-1.3.7-patched*: contains the patched version of TORCS with scr_server and screenpipe to stream the game image, based on ETCD.
* *etcd*: contains the ETCD source, updated in order not to store WAL files locally (not needed for the purposes of this project).
* *etcd_original*: contains the original version of the ETCD source, used for comparison with the updated version.

Representation of the current state of the modularization:
<br />
<img src="https://user-images.githubusercontent.com/16836365/188315972-fe3a0394-eaa0-4339-94b0-4c0e99416fef.jpg" alt="drawing" width="60%"/>

## Compatible operating systems
As of now, the application has been tested to be working only on Docker instances run in a Linux environment.

## Pre-run script
In order to allow the Docker containers to access the local display, the following script should be run: <br/>
`sudo xhost local:root`

## Run the Docker images
In order to run the Docker images provided in the `TORCS-docker-1.3.7` directory, one should look at the nested folders and run the command `docker-compose up` in order to launch the related containers. In particular, for the *ETCD version*:
* *app-etcd*: contains the main TORCS application, the musicplayer and the ETCD container image. These should be the first images to be initialized.
* *client*: contains the ETCD version of the scr client to be connected to the main TORCS application, after either a "Quick race" or "Practice" has been started.
* *watcher*: contains the ETCD version of the screenpipe client, which is able to connect to the main TORCS application, obtain the game image and render it in a dedicated window.
<a/>

For the *Redis version*:
* *app-redis*: contains the main TORCS application, the musicplayer and the Redis container image. These should be the first images to be initialized.
* *client-redis*: contains the Redis version of the scr client to be connected to the main TORCS application, after either a "Quick race" or "Practice" has been started.
* *watcher-redis*: contains the Redis version of the screenpipe client, which is able to connect to the main TORCS application, obtain the game image and  render it in a dedicated window.
