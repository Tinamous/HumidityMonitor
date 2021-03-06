3D Printer Filament Humidity Controlled Storage
===============================================

![Filament storage boxes in use](/Pictures/MakespaceFilament.png)

Comprises of:

* Outer box (19L XL Really Useful Box).
* Laser cut acrylic silica gel case
* Particle Photon based humidity sensing and fan control PCB inside silica gel case.
* Self indicating silica gel inside #2.

1) Outer box
---------------------
A 19L XL A4 suspension folder Really Useful Box works well. Be sure to get the one with the high lid. (not the regular 19L box).

* Needed: Staples link to case.
* Maplin: http://www.maplin.co.uk/p/really-useful-19l-xl-box-n68fl (Untested but it's a lot cheaper than the staples one!)

The lid of the 19L XL case will need to be drilled to hold the acrylic silica gel cartridge in place. Do this before gluing the cartridge so the acrylic can be used as a template.

Place what is labeled as the top piece in the designs (actually the bottom piece with all the holes in it) onto the lid and mark/drill the 4 corner screws.

If you wish for the power to go in via the top (prevents boxes being stacked but is obvious that it is powered so prevents people missing it) use the large bottom piece (actually top) with the larger round hole in it and align the two corner screws then drill out the larger hole in the lid.

Power connectors were sourced from eBay (http://www.ebay.co.uk/itm/PLUGZ2GO-2-1MM-X-5-5MM-PANEL-MOUNT-DC-PLUG-SOCKET-JACK-CHOICE-OF-COLOUR-/221456273750?var=&hash=item338fd29156:m:m85llLg3q2OnjM6yf-dwyPw), be sure you can get the same before drilling the power connector hole in the lid. 

Alternativly power connection may be done from the side of the lid by running the power cable out of either top or bottom gel case holes, this would allow the boxes to be stacked, although it makes it less obvious theirs a cable connected so care needs to be taken.

M4 x 70mm machine screws with washer+wingnut on the lit are used to hold the actylic case to the lid. (See notes below on re-filling the silica gel)

2) Laser Cut Acrylic Gel Case
-----------------------------
5mm clear acrylic cut as detailed in Hardware\Case\LaserCut

(You can use another colour obviously but the clear is cheaper and allows the self indicating gel to be seen).

The case contains a 40mm fan. A standard 3 pin PC fan should be used with a filter to prevent silica gel getting in to it.


3) Humidity Monitor.
---------------------

Use Particle Photon (or can use Particle Core device).

See Hardware\PCB\V2

Don't use V1 as the FET has Source/Drain the wrong way around.

PCB available from OSHPark: https://oshpark.com/shared_projects/nOVDuNCE

JP3 or JP4 are used for the HTU21 sensor. SparkFun and eBay boards have swapped SDA/SCL, watch for correct version.
* JP3 is for the eBay version
* JP4 is for SparkFun version.

Build using build.particle.io, see source folder.

A 12V power supply us needed capable of supplying upto 500mA (depends on the fan).

If a 5V fan is used a 5V PSU can be used and the onboard regulator not fitted, instead connecting the Vin to VOut holes with a piece of wire.

The PCB is mounted to the case with 4x 10mm plastic standoffs and 8x M3x8mm bolts from either side.


4) Self indicating silica gel
-----------------------------
Almost any silica gel can be used and doesn't need to be self indicating but it helps with seeing when the gel needs to be dried and when drying the gel that it is ready.

Gel should be stored in a sealed box/bag when not in use.

Gel can be re-generated by heating in an electric (untested on gas) oven for about 3 hours at 110�C


5) Connection to Tinamous
-------------------------
Optional step. Tinamous supports Particle devices by use of the ParticleBot in Tinamous and will automatically bring in and store devices/measurements so the humidity can be remotely monitored.

PLA Filament storage 24 hour humdity chart:

![Humidity last 24 hours](/Pictures/PlaHumidity.png)


PLA Filament storage 7 day humidity chart with change of gel:

![Humidity last 7 days](/Pictures/PlaStorage-7Days_ChangeGel.png)


Notes
=====

Replacing the silica gel.
--------------------------

This needs to be done regularly (depending on usage), ca. every month and the silica gel dried (110C 3hrs spread out on a baking tray). I typically replace when it gets to about 20% humidity in the box.

The silica gel can be dried and re-used.

Silica gel will try to go everywhere.

* Remove acrylic case from box lid (place on flat surface, undo wing nuts on top of box.
* Remove lid upwards leaving acrylic case on flat surface.
* Replace 2x win nuts onto acrylic case at PCB end to hold the large top piece in place
* Remove small top piece from top to expose silica gel
* Remove 2x screws that were holding the top peice in
** failure to remove these now will result in silica gel getting wedged behind them and stopping correct alignment.
* Pour gel into a container (suggest Sistem 2.35L tub - http://www.amazon.co.uk/gp/product/B005D6Y4IA?psc=1&redirect=true&ref_=oh_aui_detailpage_o03_s00)
* Put funnel into opening
* Slowly pour fresh silica gel into acrylic case, shaking to move the gel down the case as you go
* Do not fill completly full, allow 1/2-1 inch space when the case is closed up and stood tall 
* Re-Fit small lid peiece
* Holding top in place, stand case tall to allow silica gel to fully move down to the fan and make re-fitting screws easier.
* Refit 2x screws for the small lid piece.
* Stand back down, remove 2x wingnuts to allow lit to be fitted.
* Fit box lit and re-fit wing nuts.
