# TORCS_multi_docker
## Repository structure
This repository contains the source code of a project aimed toward the modularization of the TORCS racing game simulator. The content of the repository is organized as follows:
* *torcs-1.3.7-patched*: contains the patched version of TORCS with scr_server and screenpipe to stream the game image.
* *scr-client-cpp*: contains the scr client used to connect and play TORCS in "Quick game" or "Practice" modes.
* *TORCS-docker-1.3.7*: contains the docker-compose.yml and Dockerfile of the 4 containers used for the project (app-etcd, watcher, client).

Representation of the current state of the modularization:
<br />
<img src="https://user-images.githubusercontent.com/16836365/188315972-fe3a0394-eaa0-4339-94b0-4c0e99416fef.jpg" alt="drawing" width="60%"/>

## Compatible operating systems
As of now, the application has been tested to be working only on Docker instances run in a Linux environment.

## Pre-run script
In order to allow the Docker containers to access the local display, the following script should be run: <br/>
`sudo xhost local:root`

## Run the Docker images
In order to run the Docker images provided in the `TORCS-docker-1.3.7` directory, one should look at the nested folders and run the command `docker-compose up` in order to launch the related containers. In particular:
* *app-etcd*: contains the main TORCS application and the ETCD container image. These should be the first images to be initialized.
* *client*: contains the scr client to be connected to the main TORCS application, after either a "Quick race" or "Practice" has been started.
* *watcher*: contains the screenpipe client, which is able to connect to the main TORCS application, obtain the game image and (very roughly) render it in a dedicated window.
