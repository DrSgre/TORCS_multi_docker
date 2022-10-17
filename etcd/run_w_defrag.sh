#!/bin/bash
sudo cp -r ../etcd /etcd
/etcd/bin/etcd &
sleep 2
/etcd/periodic_defrag.sh
