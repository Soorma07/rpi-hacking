Raspberry Pi hacking
====================

Here are some things I want to do with the Raspberry Pi.

* [Musical instrument][instr]
* Some ideas for embedded programming discussed in [my blog][jsos].

[jsos]: http://willware.blogspot.com/2011/06/somewhat-half-baked-embedded-os-idea.html
[instr]: https://en.wikipedia.org/w/index.php?title=User:WillWare/Raspberry\_Pi\_hacking

The latter idea will require more thinking. I think the smart thing to do is run NodeJS
on the RPi, probably using [this approach][rpi-nodejs-hw]. I want to make it a good way
to program hardware so I also envision a [kernel module][kernelmodule] that provides
access to physical address space, probably thru an [ioctl][ioctl] call.

[rpi-nodejs-hw]: https://gist.github.com/3301813
[kernelmodule]: http://www.tldp.org/LDP/lkmpg/2.6/html/
[ioctl]: http://en.wikipedia.org/wiki/Ioctl
