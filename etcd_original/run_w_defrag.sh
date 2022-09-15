#!/bin/bash
./bin/etcd &
sleep 2
./periodic_defrag.sh
