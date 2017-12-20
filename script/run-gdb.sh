#!/bin/bash

while ! nc -z localhost 1234; do echo "Waiting for gdb server"; sleep 1; done

docker run -v $PWD:/opt/kos -it --detach-keys="ctrl-u,u" bellkev/kos gdb -x script/gdb-init
