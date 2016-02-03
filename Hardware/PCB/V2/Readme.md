Filament Box IoT Humidity Sense and Control
---------------------------------------------

Used to monitor the humidity level in a 3D filament storage box and re-circulate the air through a silica gel cartridge.

Notes:
* 7805 Regulator should be replaces with a switching "780x replacement" type device to keep heat down. The 7805 will get very hot dropping 12v to 5v.
* JP3 and 4 are variants of connectors for HTU21D breakout boards.
** JP4 should be used with the Spark Fun version (https://www.sparkfun.com/products/12064)
** JP3 should be used with the common eBay version (SDA/SCL swapped from Spark Fun version).
* Normal use is intended for 12V with a 12V fan. However if using a 5V fan the supply may be dropped to 5V and IC1 (7805) not fitted, instead pads 1 & 3 connected to feed 5V directly into the photo's 5V supply pin.
* Do not use photon USB B connector to provide 5V for fan, it will not supply the full 5v and may not have PCB traces strong enough for the fan current.

Untested at this time. Awaiting PCBs.