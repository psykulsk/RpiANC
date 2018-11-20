#!/bin/bash
rsync -avz --cvs-exclude ../../RpiANC --exclude "RpiANC/cmake-build-debug" --exclude "RpiANC/cmake-build-release" --exclude "RpiANC/Scripts/audio_files" --exclude "RpiANC/Makefile" --exclude "RpiANC/OctaveCode" --exclude "RpiANC/.idea" ../../RpiANC pi@192.168.0.155:./ \
&& ssh pi@192.168.0.155 "cd ~/RpiANC && make ffANC"
ssh pi@192.168.0.155 "cd ~/RpiANC && ./ffANC" &
timeout 15 play ./audio_files/400Hz_test.mp3
#pkill mpsyt && \
#pkill mpv && \
rsync -avz pi@192.168.0.155:~/RpiANC/rec/ ./audio_files
pipenv run ./plot_fb_test_results.py &
#pipenv run ./play_raw_audio.py audio_files/err_mic.dat


