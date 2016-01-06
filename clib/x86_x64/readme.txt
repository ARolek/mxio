A. What is MXIO Linux Library for x86 and x64?
======================================
     MXIO Linux Library is designed for programmers accessing remote digital and analog data
 from Moxa iologik 4000 I/O Server, Moxa iologik 2000 I/O Server,Moxa iologik 4200 I/O Server,
 Moxa iologik 5000 I/O Server, ioLogik 1200 I/O Server and and ioLogik R1000 I/O Server module
 over an Ethernet or RS-485 network.
 MXIO Linux Library for x86 and x64,(mxio-library-x.x.x.x.tgz), supports general x86 and x64 platforms which with
 linux of core 2.6.
 Please be noticed that Click&Go command is only used for ioLogik 2000,ioLogik 4200 and ioLogik 5000 Ethernet series

B. Supported Platforms
======================
   mxio-library-x86-x64-x.x.x.x.sh	V	Support x86 and x64 platform, such as Fedora 16_x86_64, Debian 6.0.4 amd64, ....

C. Supported I/O Modules
========================
      MXIO Linux Library supported below I/O modules.
      ioLogik 4000 series: NA-4010, NA-4020, NA-4021
                           M-1400, M-1401, M-1410, M-1411, M-1800, M-1801, M-1600, M-1601, M-1450, M-1451
                           M-2400, M-2401, M-2800, M-2801, M-2600, M-2601, M-2402, M-2403, M-2404, M-2405, M-2250, M-2254
                           M-3400, M-3401, M-3402, M-3403, M-3410, M-3411, M-3412, M-3413, M-3414, M-3415
                           M-6200, M-6201
                           M-4201, M-4202, M-4210, M-4211, M-4212
      ioLogik 2000 series: E2210, E2212, E2240, E2260, R2110, R2140, E2214, E2242, E2262
      ioLogik 4200 series: E4200
                           M-1400, M-1401, M-1410, M-1411, M-1800, M-1801, M-1600, M-1601, M-1450, M-1451
                           M-2400, M-2401, M-2800, M-2801, M-2600, M-2601, M-2402, M-2403, M-2404, M-2405, M-2250, M-2254
                           M-3400, M-3401, M-3402, M-3403, M-3410, M-3411, M-3412, M-3413, M-3414, M-3415
                           M-6200, M-6201
                           M-4201, M-4202, M-4210, M-4211, M-4212
      ioLogik 5000 series: W5340, W5312
      ioLogik 1200 series: E1210, E1211, E1212, E1214, E1240, E1241, E1242, E1260, E1262, E1261-WP-T, E1261H, E1263H, E1213
      ioLogik 1500 series: E1510, E1512
      ioLogik R1000 series: R1210, R1212, R1214, R1240, R1241

D. File List:
=============
   x86 and x64 platform
   ../                             API_SUPPORT_LIST_*.pdf
   ../                             MXIO DLL API Reference (mxio.chm)
   ../installer/                   mxio-library-x.x.x.x.sh
   ../examples/ethernet/           ethernet.cpp
                                   makefile
   ../examples/serial/             serial.cpp
                                   makefile
   ../examples/message/            Message.cpp
                                   makefile
   ../examples/activetag/          Activetag.cpp
                                   makefile
   ../examples/changedupip/        ChangeDupIP.cpp
                                   makefile
E. Installation of MXIO Linux Library:
======================================
   For General x86 and x64 platform:
   Step1. To install the Library, unpacked mxio-library-x.x.x.x.tgz on your host PC
          # cd /
          # mkdir mxio
          # cd mxio
          # cp /mnt/cdrom/mxio-library-*.tgz .
          # tar zxvf mxio-library-*.tgz
          # cd /installer
   Step2. Next, run the following script as root, to install the libraries and header file in the /usr/local directory:
          # ./mxio-library-x86-x64-*.sh
   Step3. The Library installation will take a few minutes to complete.
   Step4. Programming your program on your development envelopment.





