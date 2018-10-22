#!/bin/bash
rsync -avz --cvs-exclude ../../RpiANC --exclude "RpiANC/cmake-build-debug" --exclude "RpiANC/cmake-build-release" --exclude "RpiANC/Makefile" --exclude "RpiANC/OctaveCode" --exclude "RpiANC/.idea" --exclude "RpiANC/Scripts/audio_files" ../../RpiANC pi@192.168.0.155:./ \
&& ssh pi@192.168.0.155 "cd ~/RpiANC && make ffANC" 
mpsyt 'playurl https://www.youtube.com/watch?v=D00TqNbMDNI' &
#mpsyt 'playurl https://www.youtube.com/watch?v=vuZ4uP279oc' &
ssh pi@192.168.0.155 "cd ~/RpiANC && ./ffANC" && \
pkill mpsyt && \
pkill mpv && \
rsync -avz pi@192.168.0.155:~/RpiANC/rec/ ./audio_files
pipenv run ./plot_test_results.py &
#pipenv run ./play_raw_audio.py audio_files/err_mic.dat

