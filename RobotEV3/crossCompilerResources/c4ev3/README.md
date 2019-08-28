# How to update c4ev3

When c4ev3 is updated you need to rebuild the static library (2 times, for x86 and for raspberry pi) and update the headers files inside ora-cc-rsc.
Use the `copy_c4ev3.sh` script to automate the procedure.

**Scripts arguments**

1) root folder of c4ev3 (EV3-API).
2) prefix of the cross-compiler for arm built with glibc: if you installed the cross compiler using `sudo apt-get install gcc-arm-linux-gnueabi` the prefix is `arm-linux-gnueabi-`.
3) prefix of the cross-compiler for arm built with uclibc.
