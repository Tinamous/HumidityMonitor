use <MCAD/boxes.scad>

fanSize = 40;
fanHeight = 35;

// Overall size
height = 4;
thickness = 2;
length = 90;
width = 70;

outerWallWidth = 3;
radius = 4;
drillHoldRadius = 2; // 4mm Hole

$fn=100;

// -----------------------------------------
// -----------------------------------------
module GenericBase(xDistance, yDistance, zHeight) {
	translate([radius,radius,0]) {
		minkowski()
		{
			cube([xDistance-(radius*2), yDistance-(radius*2), (zHeight/2)]);
			cylinder(r=radius,h=(zHeight/2));
		}
	}
}

// -----------------------------------------
// Show a fan box in the unit to give perspective
// -----------------------------------------
module ShowFan() {
    xOffset = (width - fanSize)/2;
    // 5mm fudge to allow for fan dust cap
    yOffset = length - ((radius* 2 )) - fanSize - 5; 
    translate([xOffset,yOffset,0]) {
        %cube([fanSize,fanSize, fanHeight]);
    }
}


// -----------------------------------------
// 
// -----------------------------------------
module OuterWall(base) {
    difference() {
        union() {
            GenericBase(width,length,height);
        }
        union() {
            translate([outerWallWidth,outerWallWidth,thickness]) {
                GenericBase(width - (outerWallWidth*2),length  - (outerWallWidth*2),height);
            }
        }
    }
}

// -----------------------------------------
// Holes for screws to go into to match up with posts.
// -----------------------------------------
module PostHoles() {
    union() {
            translate([radius,radius,-1]) {
                #cylinder(h=height+2, r=drillHoldRadius,center=false);
            }
            translate([width-radius,length-radius,-1]) {
                #cylinder(h=height+2, r=drillHoldRadius,center=false);
            }
            translate([width-radius, radius,-1]) {
                #cylinder(h=height+2, r=drillHoldRadius,center=false);
            }
            translate([radius,length-radius,-1]) {
                #cylinder(h=height+2, r=drillHoldRadius,center=false);
            }
        }
}

// -----------------------------------------
// Create a series of airvents to allow the air drawn by the fan through.
// -----------------------------------------
module AirVent(xPostion) {
    
// Offset from the edges of the vents
ventStartOffset = 8;
ventEndOffset = length - (2*ventStartOffset);
ventWidth = 1; // Silica is about 2mm.
    
    translate([xPostion,8,0]) {
        #cube([ventWidth,ventEndOffset,4], center = false);
    }
}

module AirVents() {
    startPosition = 8;
    endPosition = width - startPosition;
    stepSize = 3;
    
    for (xOffset = [startPosition : stepSize : endPosition]) {
    AirVent(xOffset);
    }
}

// -----------------------------------------
// -----------------------------------------
module Debug() {
    ShowFan();
    difference() {
        union() {
            OuterWall();
        }
        union() {
            PostHoles();
            #AirVents();
        }
    }
}

// Comment out when building, uncomment for debug.
Debug();