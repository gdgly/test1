export LD_LIBRARY_PATH=/tmp:$LD_LIBRARY_PATH
usleep 200000
/tmp/x11vnc -noipv6 -rawfb /dev/fb0 -pipeinput UINPUT:touch,direct_abs=/dev/input/event1,nouinput,dragskip=4 -clip 320x320+0+0
