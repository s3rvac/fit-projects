#!/bin/sh

stty -F /dev/ttyS0 9600
stty -F /dev/ttyS1 9600
stty -F /dev/ttyS0 clocal cread -crtscts cs8 -cstopb hup -parenb parodd -brkint -icrnl ignbrk -igncr ignpar imaxbel -inlcr inpck -istrip -iuclc -ixany ixoff -ixon bs0 cr0 ff0 nl0 -ocrnl -ofdel -ofill -olcuc -onlcr -onlret onocr -opost tab0 vt0 -crterase crtkill -ctlecho -echo -echok -echonl -echoprt -icanon -iexten -isig -noflsh -tostop -xcase time 5 min 1
stty -F /dev/ttyS1 clocal cread -crtscts cs8 -cstopb hup -parenb parodd -brkint -icrnl ignbrk -igncr ignpar imaxbel -inlcr inpck -istrip -iuclc -ixany ixoff -ixon bs0 cr0 ff0 nl0 -ocrnl -ofdel -ofill -olcuc -onlcr -onlret onocr -opost tab0 vt0 -crterase crtkill -ctlecho -echo -echok -echonl -echoprt -icanon -iexten -isig -noflsh -tostop -xcase time 5 min 1
