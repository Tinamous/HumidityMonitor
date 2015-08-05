use <MCAD/boxes.scad>

// Overall size
height = 25;
length = 90;
width = 70;

outerWallWidth = 3;
radius = 4;
drillHoldRadius = 2; // 4mm Hole

// PCB Support goes vertical 
// on both long walls.
pcbSupportHeight = height;

// How far into the center the supports go.
// this wifth includes the wall width as well.
// so 6mm support gives actual 6-3 (outerWallWidth) = 3mm into box.
// width = 70.
// Arduino needs 52
// 9mm supports either size
pcbSupportWidth = 9; // 
pcbSupportLength = 6; //(6-pcbHeight)/2 gives size either size of PCB - about 2mm
pcbHeight = 2.0; // 1.6mm PCB

// PCB Max 62mm x 45mm max (x 1.6mm) - actual length of PCB
// Ardiono is 53.5 mm wide
// needs seperation between supports of 51-52mm
pcbWidth = 62; 
// Max pcbWidth is govened by the support height.
pcbYOffset = 20;
$fn=100;

// -----------------------------------------
// -----------------------------------------
module GenericBase(xDistance, yDistance, zHeight) {
    
// -----------------------------------------
// Show a fan box in the unit to give perspective
// -----------------------------------------
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
// 
// -----------------------------------------
module MakeBox() {
    
    difference() {
        union() {
            GenericBase(width,length,height);
        }
        union() {
            translate([outerWallWidth,outerWallWidth,-2]) {
                GenericBase(width - (outerWallWidth*2),length  - (outerWallWidth*2),height+2);
            }
        }
    }
}

// -----------------------------------------
// Mounting posts
// -----------------------------------------
module Posts() {
    
   
    union() {
        translate([radius,radius,0]) {
            cylinder(h=height, r=radius,center=false);
        }
        translate([width-radius,length-radius,0]) {
            cylinder(h=height, r=radius,center=false);
        }
        translate([width-radius, radius,0]) {
            cylinder(h=height, r=radius,center=false);
        }
        translate([radius,length-radius,0]) {
            cylinder(h=height, r=radius,center=false);
        }
    }
}

// -----------------------------------------
// Create holes in posts for screws
// Using M3 inserts from InsertsDirect
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

module AddSensorWall() {
    // Add compartment for sensor.
    translate([0, 20,0]) {
        cube([width, 2, height]);
    }
}

// -----------------------------------------
// -----------------------------------------
module Debug() {
    difference() {
    union() {
        MakeBox();
        AddSensorWall();
        Posts();
    }
    union() {
        PostHoles();
    }
}
}

// Comment out when building, uncomment for debug.
Debug();