use <MCAD/boxes.scad>

// Spool width = 55m.
// 5 spools = 6 partitions
// partition 3mm
// 6x3 = 18mm

numberOfSpools = 5;
spoolWidth = 55;
partitionWidth = 3; // 3mm acrylic

// Overall size 
// make 1/2 width to allow for one either side
width = 5; //((spoolWidth - ((numberOfSpools+1) * partitionWidth)) / 2) /2;
outerRadius = 20; 
innerHoleRadius = 11;

$fn=50;

// -----------------------------------------
// RodSupport
// -----------------------------------------
module Padding() {
mountHoleOffset = outerRadius*(2/3);
    
    difference() {
        union() {
            cylinder(r=outerRadius, h=width,center=false);
        }
        union() {
            // Inner hole for rod.
            #cylinder(r=innerHoleRadius, h=width,center=false);
            
            translate([0,-innerHoleRadius,0]) {
                #cube([outerRadius,innerHoleRadius*2,width]);
            }
            
            // cube so that the padding can be slipped on once rod is inplace.
        }
    }
}


// -----------------------------------------
// -----------------------------------------
module Debug() {
    union() {
        Padding();
    }
}

// Comment out when building, uncomment for debug.
Debug();