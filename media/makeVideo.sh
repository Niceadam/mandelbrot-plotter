#!/bin/bash
ffmpeg -framerate 25 -i image%d.png mandel.mp4
