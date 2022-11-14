#!/bin/bash
sudo cp -r ../etcd /ff
/ff/etcd/bin/etcd &
sleep 2
/ff/etcd/periodic_defrag.sh
