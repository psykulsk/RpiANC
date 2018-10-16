#!/bin/bash
rsync -avz --cvs-exclude RpiANC --exclude "RpiANC/cmake-build-debug" --exclude "RpiANC/cmake-build-release" --exclude "RpiANC/Makefile" --exclude "RpiANC/OctaveCode" --exclude "RpiANC/.idea" RpiANC pi@192.168.0.155:./ \
&& ssh pi@192.168.0.155 "cd ~/RpiANC && make ffANC" 
mpsyt 'playurl https://www.youtube.com/watch?v=D00TqNbMDNI' &
ssh pi@192.168.0.155 "cd ~/RpiANC && ./ffANC" && \
pkill mpsyt && \
pkill mpv && \
rsync -avz pi@192.168.0.155:~/RpiANC/rec/ ./audio_files


