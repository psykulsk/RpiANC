#!/bin/bash

rsync -avz --cvs-exclude ../../RpiANC --exclude "RpiANC/cmake-build-debug" --exclude "RpiANC/cmake-build-release" --exclude "RpiANC/Scripts/audio_files" --exclude "RpiANC/Makefile" --exclude "RpiANC/OctaveCode" --exclude "RpiANC/.idea" ../../RpiANC pi@raspberry:./
