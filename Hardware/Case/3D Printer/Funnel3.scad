radius = 5;
$fn=200;

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
    
    
    baseToFunnelHullConnection = 1;
    baseBoxHeight = 55 - baseToFunnelHullConnection;
    funnelTopHeigth = 30;
    hullConnectionHeight = 30; //baseBoxHeight; //40;
   
    difference() {
        GenericBase(baseWidth, baseDepth, baseBoxHeight);
    }
       
    hull() {
       
        // box section
        translate([0,0,baseBoxHeight]) {
            // This connects the base box to the cylinder at the top through
            // the hull connection
            GenericBase(baseWidth, baseDepth, baseToFunnelHullConnection);
        }
        
        // Align the center of the base cube
        // With the center of the cylinder
        baseXCenter = baseWidth /2 ;
        baseYCenter = baseDepth / 2;
        
                   
        // Top of the funnel
        //translate([-((topWidth-baseWidth)/2),-((topDepth-baseDepth)/2),20+20]) {
        translate([baseXCenter,baseYCenter,hullConnectionHeight+baseBoxHeight]) {
            //GenericBase(topWidth, topDepth, 20);
            cylinder(d=topWidth, h=funnelTopHeigth, $fn=80);
        }
    }
        
}



wallThickness = 2;

baseWidth = 85; //70;
baseDepth = 44; //40;
topWidth = 120;
topDepth = 40;

difference() {
    
    // Funnel Outer
    // 100x44
    Funnel(baseWidth,baseDepth, topWidth, topDepth);
    
    union() {
    
        // Subtract the inner.
        translate([wallThickness,wallThickness,0]) {
            Funnel(baseWidth-(wallThickness*2),
                baseDepth-(wallThickness*2), 
                topWidth-(wallThickness*2), 
                topDepth-(wallThickness*2));
        }
        
        // Cut out a section so that the funnel
        // can sit and still allow stuff to flow out
        #cube([baseWidth,6,40]);
    }
}