#!/bin/bash
./sync2pi.sh && ssh pi@raspberry "cd ~/RpiANC/build && make ffANC"
#play ./audio_files/400Hz_test.mp3 &
ssh pi@raspberry "cd ~/RpiANC/build && ./ffANC" && \
# pkill play
rsync -avz pi@raspberry:~/RpiANC/build/rec/ ./audio_files
pipenv run ./plot_ff_test_results.py &
#pipenv run ./play_raw_audio.py audio_files/err_mic.dat


