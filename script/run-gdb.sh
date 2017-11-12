#!/bin/bash

while ! nc -z localhost 1234; do echo "Waiting for gdb server"; sleep 1; done
docker run -v $PWD:/root -it --detach-keys="ctrl-e,e" bellkev/kos bash -c 'gdb -x script/gdb-init'
