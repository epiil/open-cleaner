//Opencleaner - Precision Roller Guide
//Revised 5 Jul 2015
//Designed for 1/4" shaft
//in milimeters, inches are preferred


$fn=100;

module roller() {

	difference() 
	{
		union() 
		color("black")
		{
		cylinder(h=20.828, d=21.20, $fs=.01); // .820" total width .80 Inner width + 
		cylinder(r=21.20, h=2.54, $fs=.01); //bottom flange, .1"
		translate([0,0,20.574]) //.810" height to top flange
		cylinder(r=21.20, h=2.54, $fs=.01); //top flange, .1"
		}
		union()
		color("black")
		{	
		cylinder(h=80, d=12.7, center = true, $fs=.01); //shaft dimension, 1/2" inch
		cylinder(h=7.239, d=19.0246, $fs=.01); //bottom bearing offset, .285" wide, .749" diameter
		cylinder(h=2, d=19.05, center=true, $fs=.01); // overcuts bottom bearing offset
		translate([0,0,13.589]) //.535" height to bearing offset
		cylinder(h=7.239, d=19.0246, $fs=.01); //top bearing offset, .285" wide, .749" diameter
		translate([0,0,20]) //overcuts top bearing offset
		cylinder(h=7.239, d=19.0246, $fs=.01); //top bearing offset, .285" wide, .749" diameter
		}
	}
}

roller();