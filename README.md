Raspberry Pi hacking
====================

Here's how I like to set up the RPi. First, much as it pains me to say it, it's
actually a good idea to use a Windows machine to initially prep the SD card.
Then:

    rpi$ raspi-config
    # Select "boot_behaviour" and tell it not to start the desktop on boot.
    # Select "expand-rootfs" and expand to the size of the SD card.

    rpi$ sudo apt-get update
    rpi$ sudo apt-get install vim build-essential git git-core
    host$ cat ~/.ssh/id_dsa.pub | ssh pi@rpi "cat >> /home/pi/.ssh/authorized_keys"
    rpi$ vi .bashrc     # add "export EDITOR=vim" or whatever editor you like

I install git on the RPi, but if I'm using it there, I'm doing something wrong.

Here are some things I want to do with the Raspberry Pi.

* [Musical instrument][instr]
* Some ideas for embedded programming discussed in [my blog][jsos].

[jsos]: http://willware.blogspot.com/2011/06/somewhat-half-baked-embedded-os-idea.html
[instr]: https://en.wikipedia.org/w/index.php?title=User:WillWare/Raspberry\_Pi\_hacking

The latter idea will require more thinking. I think the smart thing to do is
run NodeJS on the RPi, probably using [this approach][rpi-nodejs-hw].

[rpi-nodejs-hw]: https://gist.github.com/3301813

I want to be able to write a [kernel module][kernelmodule] that provides access
to physical address space, probably thru an [ioctl][ioctl] call. I need that
for both projects.

[kernelmodule]: http://www.tldp.org/LDP/lkmpg/2.6/html/
[ioctl]: http://en.wikipedia.org/wiki/Ioctl

Initially I figured I'd SSH into the RPi and do development there, but it will
be a lot simpler if I can build the kernel module on a desktop or laptop.  I'm
using stuff from
https://www.grendelman.net/wp/cross-compiling-kernel-modules-for-raspbian/. It
seems to be working fine. Ultimately it would be cool to be able to build an
entire SD card image.

Cross-compiling a kernel and modules
------------------------------------

On your host machine, run the makekernel.sh script. It will take a long time.
Then copy the kernel over to the RPi.

    host$ scp linux/arch/arm/boot/Image pi@rpi:/home/pi
    rpi$ sudo cp Image /boot/kernel.img

Now go into the kernel-module directory and use the foo.sh script to build the
kernel module. If you just run "make" you'll get a kernel module for your host
machine and it won't work on the RPi. Then copy it over and run it

    host$ scp  pi@rpi:/home/pi
    rpi$ sudo insmod hello-1.ko
    rpi$ dmesg     # Look for "Hello world" message
    rpi$ sudo rmmod hello-1
    rpi$ dmesg     # Look for "Goodbye world" message

When you install the char\_dev driver, you'll want to define a /dev node for it:

    rpi$ sudo mknod /dev/char\_dev c 100 0
    rpi$ sudo chmod 777 /dev/char\_dev

Installing NodeJS on the RPi
----------------------------

I worked from the advice at https://gist.github.com/3301813 to install NodeJs
successfully, basically doing this. My installation differs a little bit
because NodeJS had matured from 0.8.8 to 0.8.14 since that information was
originally posted. Run these commands on the RPi (an exception to the maxim
that using git on the RPi indicates a mistake):

    rpi$ git clone https://github.com/joyent/node.git
    rpi$ cd node
    rpi$ git checkout v0.8.14-release
    rpi$ ./configure && make
    rpi$ python tools/test.py -t 120 --mode=release simple message  # <-- optional
    rpi$ sudo make install

