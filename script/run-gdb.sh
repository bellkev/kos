#!/bin/bash

while ! nc -z localhost 1234; do echo "Waiting for gdb server"; sleep 1; done

if [[ -z $PAUSE ]]; then
    GDBFLAGS="-ex continue"
fi

docker run -v $PWD:/root -it --detach-keys="ctrl-e,e" bellkev/kos gdb -x script/gdb-init $GDBFLAGS
