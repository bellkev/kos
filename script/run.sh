#!/bin/bash

set -e

tmux new-window -n KOS "cd $PWD && MODE=$MODE script/run-vm.sh; bash"
tmux split-window -t KOS -v -p 66 "cd $PWD && PAUSE=$PAUSE script/run-gdb.sh"
tmux split-window -t KOS -v -p 50 "while [[ ! -f $PWD/kernel.log ]]; do echo 'Waiting for log file...'; sleep 1; done; tail -f $PWD/kernel.log"
