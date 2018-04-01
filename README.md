# Command line mediaplayer

Play videos and other media in the command line.

## Usage



## Dependencies

| Library        | Version        | Details           |
| -------------- | -------------- | ----------------- |
| libncurses     | 6.0.20160213   | I assume that the program will compile against newer versions as well. |
| libavcodec     | 56/60/100      | MAJOR/MINOR/MICRO |
| libavformat    | 56/40/101      | MAJOR/MINOR/MICRO |
| libjpeg-turbo  |                | TODO: Enter working version number. I assume that the program will compile against any new version. |

## FFmpeg documentation

Use documentation for FFmpeg version 2.8.

http://www.ffmpeg.org/doxygen/2.8/

## Compilation

Generally, make sure you have installed all required dependencies, and then run `make`.

### Ubuntu 16.04

```
sudo apt install libjpeg-turbo8-dev
sudo apt install libavcodec-dev
sudo apt install libavformat-dev
sudo apt install libncurses5-dev
make
```

## TODO

- Colours
- Subtitles
- Audio
- Support for more image formats
