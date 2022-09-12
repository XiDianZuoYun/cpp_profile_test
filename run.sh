#!/usr/bin/zsh
nohup ./writer > writer.out 2>&1 &
nohup ./reader > reader.out 2>&1 & 
