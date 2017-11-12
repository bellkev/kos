#!/bin/bash

tmux new-window -n KOS "cd $PWD && script/run-vm.sh" && tmux split-window -t KOS -v "cd $PWD && script/run-gdb.sh || bash"
