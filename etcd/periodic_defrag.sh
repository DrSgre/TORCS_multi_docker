#!/usr/bin/env bash

while :
do
	ETCDCTL_API=3 etcdctl defrag
	sleep 30
done
