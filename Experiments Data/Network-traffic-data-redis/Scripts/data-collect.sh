#!/bin/bash

timestamp() {
  date +"%T" # current time
}

while :
do
    timestamp
    docker stats --no-stream --format "{{.Name}}-{{.NetIO}}-{{.BlockIO}}" >> output.txt
    echo "" >> output.txt
done
