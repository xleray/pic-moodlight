This archive contains a copy of the Microchip TCP/IP stack, modified to work on various Olimex produced pic-based ethernet boards. currently the stack version is 4.02 and the supported boards include:

PIC-MAXI-WEB 	- http://www.olimex.com/dev/pic-maxi-web.html
PIC-WEB		- http://www.olimex.com/dev/pic-web.html
PIC-MINI-WEB	- http://www.olimex.com/dev/pic-mini-web.html
PIC-MICRO-WEB	- http://www.olimex.com/dev/pic-micro-web.html
PIC-P67J60	- http://www.olimex.com/dev/pic-p67j60.html

You need a recent versions of MPLAB IDE and the Microchip C18 compiler. Verified to work are MPLAB 7.60 + MCC18 v3.12. Student version should also work. Hi-tech compiler support has not been included but should be straightforward to implement.

Please note that the PIC-WEB and especially PIC-MINI-WEB projects are using pretty much all available RAM memory despite what the memory gauge says, so you may encounter difficulties adding functionality to these boards. If you absolutely need some extra free memory, please use the old versions of the stack for these boards. They are available here:
PIC-WEB - http://www.olimex.com/dev/soft/PIC/PIC-WEB/PIC_WEB_ver3.zip
PIC-MINI-WEB - http://www.olimex.com/dev/soft/PIC/PIC-MINI-WEB/PIC_MINI_WEB_ver3.zip