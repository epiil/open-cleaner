// tape cleaner enclosure top
// Erik Piil
//piil.erik@gmail.com
//https://github.com/epiil/tape-cleaner
//v20150310

//OUTER DIMENSIONS in mm

outerWidth=406.4;
outerLength=406.4;
outerHeight=2.286;


module top()
{
	difference() 
	{
		union() 
		{
			color("grey")
			cube(size=[outerWidth, outerLength, outerHeight], center = true, $fs=.01);
		}
		union() 
		{
			translate([114.3,114.3,0])
			cylinder(h=5, d=25.4, center = true, $fs=.01);
             translate([-114.3,-114.3,0])
			cylinder(h=5, d=25.4, center = true, $fs=.01);
             translate([-152.4,141.2875,0])
			cylinder(h=5, d=6.35, center = true, $fs=.01);
             translate([152.4,-141.2875,0])
			cylinder(h=5, d=6.35, center = true, $fs=.01);
             translate([-76.2,69.85,0])
			cylinder(h=5, d=6.35, center = true, $fs=.01);
             translate([76.2,-69.85,0])
			cylinder(h=5, d=6.35, center = true, $fs=.01);
             translate([-25.4,19.05,0])
			cylinder(h=5, d=6.35, center = true, $fs=.01);
             translate([25.4,-19.05,0])
			cylinder(h=5, d=6.35, center = true, $fs=.01);
             translate([-88.9,0,0])
			cylinder(h=5, d=6.35, center = true, $fs=.01);
             translate([88.9,0,0])
			cylinder(h=5, d=6.35, center = true, $fs=.01);
             translate([-50.8,38.1,0])
			cylinder(h=5, d=6.35, center = true, $fs=.01);
             translate([50.8,-38.1,0])
			cylinder(h=5, d=6.35, center = true, $fs=.01);
             translate([-101.6,50.8,0])
			cylinder(h=5, d=12.7, center = true, $fs=.01);
             translate([101.6,-50.8,0])
			cylinder(h=5, d=12.7, center = true, $fs=.01);
            translate([-152.4,88.9,0])
			cube(size=[50.8, 12.7, 5], center = true, $fs=.01);
		}
	}
}

top();
