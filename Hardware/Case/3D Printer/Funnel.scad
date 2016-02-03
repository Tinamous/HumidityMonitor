radius = 5;
$fn=20;

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

module HollowGenericBase(xDistance, yDistance, zHeight) {
    difference() {
        union() {
            GenericBase(xDistance, yDistance, zHeight);
        }
        union() {
            translate([2,2,0]) {
                //GenericBase(xDistance-2, yDistance-2, zHeight);
            }
        }
    }
}

// -----------------------------------------
// -----------------------------------------
module Funnel(baseWidth, baseDepth, topWidth, topDepth) {
   
    GenericBase(baseWidth, baseDepth, 20);
       
    hull() {
       
        // box section
        translate([0,0,20]) {
        GenericBase(baseWidth, baseDepth, 20);
        }
                   
        // Top of the funnel
        translate([-((topWidth-baseWidth)/2),-((topDepth-baseDepth)/2),20+20]) {
            GenericBase(topWidth, topDepth, 20);
        }
    }
        
}

difference() {
    Funnel(70,40, 100, 60);
    translate([2,2,0]) {
        Funnel(70-4,40-4, 100-4, 60-4);
    }
}