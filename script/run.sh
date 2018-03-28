#!/bin/bash

set -e

DEBUG=
MODE=iso

declare -a qemu_args

while [[ $# -gt 0 ]]; do
    case $1 in
        -d)
            DEBUG=y
            shift
            ;;
        -m)
            MODE=$2
            shift 2
            ;;
        *)
          qemu_args+=($1)
          shift
          ;;
    esac
done

if [[ $DEBUG ]]; then
    qemu_args+=("-S")
fi

tmux new-window -n KOS "cd $PWD && MODE=$MODE script/run-vm.sh ${qemu_args[*]}; bash"

if [[ $DEBUG ]]; then
    tmux split-window -t KOS -v -p 66 "cd $PWD && script/run-gdb.sh"
fi

tmux split-window -t KOS -v -p 50 "while [[ ! -f $PWD/kernel.log ]]; do echo 'Waiting for log file...'; sleep 1; done; tail -n 1000 -f $PWD/kernel.log"
