#!/bin/bash
cp -r ../etcd /run
run/bin/etcd &
sleep 2
run/periodic_defrag.sh
