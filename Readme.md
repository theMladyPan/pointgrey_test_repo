# Pointgrey_test_repo

Made for testing of pointgrey camera on any linux machine. Personaly tested on Ubuntu 17.04

## Dependencies
[PIMP_lib](https://github.com/theMladyPan/PIMP)

[flyCapture2](https://www.ptgrey.com/flycapture-sdk)

[python wrapper](https://pypi.python.org/pypi/pyflycapture2)

[flycapture minimal docker](https://cloud.docker.com/swarm/themladypan/repository/docker/themladypan/ubuntu_xenial/general)

## Setup
docker pull themladypan/ubuntu_xenial
docker run themladypan/ubuntu_xenial

inside docker:
verify that camera is accessible:
`flycap`

then run:

```git clone https://github.com/theMladyPan/PIMP
git clone https://github.com/theMladyPan/pointgrey_py_test
python test.py
```
