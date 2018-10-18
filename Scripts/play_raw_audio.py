#!/usr/bin/env python3
import sys

import pyaudio


def play_raw_file(audio_file):

    with open(audio_file, mode='rb') as file:
        data = file.read()

        audioDevice = pyaudio.PyAudio()
        stream = audioDevice.open(format=pyaudio.paInt32,
                                  channels=1,
                                  rate=44100,
                                  output=True,
                                  frames_per_buffer=1024)
        stream.write(data)
        stream.close()
        audioDevice.terminate()


if __name__ == "__main__":

    if len(sys.argv) < 2:
        print("Not enough arguments")
        exit(1)

    play_raw_file(sys.argv[1])



