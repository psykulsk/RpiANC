#!/bin/bash
rsync -avz --cvs-exclude ../../RpiANC --exclude "RpiANC/cmake-build-debug" --exclude "RpiANC/cmake-build-release" --exclude "RpiANC/Scripts/audio_files" --exclude "RpiANC/Makefile" --exclude "RpiANC/OctaveCode" --exclude "RpiANC/.idea" ../../RpiANC pi@192.168.0.156:./ \
&& ssh pi@192.168.0.156 "cd ~/RpiANC/build && make ffANC"
play ./audio_files/400Hz_test.mp3 &
ssh pi@192.168.0.156 "cd ~/RpiANC/build && ./ffANC" && \
pkill play
rsync -avz pi@192.168.0.156:~/RpiANC/build/rec/ ./audio_files
pipenv run ./plot_ff_test_results.py &
#pipenv run ./play_raw_audio.py audio_files/err_mic.dat


