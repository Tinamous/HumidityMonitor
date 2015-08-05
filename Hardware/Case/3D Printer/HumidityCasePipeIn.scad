use <MCAD/boxes.scad>

// Overall size
baseHeight = 12;
length = 30;
width = 20;

mountHoleDistance = 20;
holeOffset = (length - mountHoleDistance) /2;

radius = 4;

$fn=50;

// -----------------------------------------
// -----------------------------------------
module GenericBase(xDistance, yDistance, zHeight) {
	//roundedBox([xDistance, yDistance, zHeight], 2, true);

	// Create a rectangluar base to work from that
	// is xDistance by yDistance and zHeight height.

	// This is effectivly a cube with rounded corners

	// extend the base out by 3.5 from holes by using minkowski
	// which gives rounded corners to the board in the process
	// matching the Gadgeteer design
	
	translate([radius,radius,0]) {
		minkowski()
		{
			cube([xDistance-(radius*2), yDistance-(radius*2), (zHeight/2)]);
			cylinder(r=radius,h=(zHeight/2));
		}
	}
}

// -----------------------------------------
// Outer wall
// -----------------------------------------
module OuterWall() {
    difference() {
        union() {
            GenericBase(length,width,baseHeight);
        }
        union() {
            // Add in the screw holes.
            translate([holeOffset ,(width / 2) ,-1]) {
                #cylinder(r=2, h=baseHeight+2,center=false);
            }
            
            translate([length - holeOffset ,(width / 2) ,-1]) {
                #cylinder(r=2, h=baseHeight+2,center=false);
            }
            
            // Add in the centeral pipe connector
            translate([length/2,width / 2 ,0]) {
                #cylinder(r1=3, r2=4, h=baseHeight,center=false);
            }
        }
    }
}


// -----------------------------------------
// -----------------------------------------
module Debug() {
    union() {
        OuterWall();
    }
}

// Comment out when building, uncomment for debug.
Debug();