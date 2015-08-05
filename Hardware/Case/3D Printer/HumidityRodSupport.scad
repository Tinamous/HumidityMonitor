use <MCAD/boxes.scad>

// Overall size
// 16mm width (16 + cap = 20 - which is the room the edges take up
width = 16; // 10mm case outer, 55mm for spool holder inner
outerRadius = 25; //25 - for spool middles;
innerHoleRadius = 10;

// Cap to go over the support
capWidth = width + 4; 

holeRadius = 2; // 4 mm hole

$fn=200;

// -----------------------------------------
// RodSupport
// -----------------------------------------
module RodSupport() {
mountHoleOffset = outerRadius*(2/3);
    
    difference() {
        union() {
            cylinder(r=outerRadius, h=width,center=false);
        }
        union() {
            // Inner hole for rod.
            #cylinder(r=innerHoleRadius, h=width,center=false);
            
            // Mouting holes
            translate([mountHoleOffset,0,0]) {
                #cylinder(r=holeRadius, h=width,center=false);
            }
            translate([-mountHoleOffset,0,0]) {
                #cylinder(r=holeRadius, h=width,center=false);
            }
            translate([0,mountHoleOffset,0]) {
                #cylinder(r=holeRadius, h=width,center=false);
            }
            translate([0,-mountHoleOffset,0]) {
                #cylinder(r=holeRadius, h=width,center=false);
            }
        }
    }
}

// -----------------------------------------
// RodSupport
// -----------------------------------------
module RodCap() {
mountHoleOffset = outerRadius*(2/3);
    
    difference() {
        union() {
            cylinder(r=outerRadius+2, h=capWidth,center=false);
        }
        union() {
            // Hollow out all but 4 mm from the inside.
            // it should be a perfect fit for the rod support
            #cylinder(r=outerRadius+0.2, h=capWidth-4,center=false);
            
            // Inner hole for rod to allow for the rod being slightly 
            // longer than the supports
            #cylinder(r=innerHoleRadius+2, h=capWidth-1,center=false);
            
            // Double up on 2 of the mounting holes
            // Mouting holes
            translate([mountHoleOffset,0,0]) {
                #cylinder(r=holeRadius, h=capWidth+2,center=false);
            }
            translate([-mountHoleOffset,0,0]) {
                #cylinder(r=holeRadius, h=capWidth+2,center=false);
            }
        }
    }
}


// -----------------------------------------
// -----------------------------------------
module Debug() {
    union() {
        RodSupport();
        //RodCap();
    }
}

// Comment out when building, uncomment for debug.
Debug();