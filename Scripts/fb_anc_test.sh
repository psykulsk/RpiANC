#!/bin/bash
rsync -avz --cvs-exclude ../../RpiANC \
--exclude "RpiANC/cmake-build-debug" \
--exclude "RpiANC/cmake-build-release" \
--exclude "RpiANC/Scripts/audio_files" \
--exclude "RpiANC/Makefile" \
--exclude "RpiANC/OctaveCode" \
--exclude "RpiANC/.idea" \
../../RpiANC pi@raspberry:./ \
&& ssh pi@raspberry "cd ~/RpiANC && make ffANC" \
&& ssh pi@raspberry "cd ~/RpiANC && ./ffANC" \
&& rsync -avz pi@raspberry:~/RpiANC/rec/ ./audio_files
pipenv run ./plot_fb_test_results.py &
#play ./audio_files/400Hz_test.mp3
#pkill mpsyt && \
#pkill mpv && \
#pipenv run ./play_raw_audio.py audio_files/err_mic.dat


