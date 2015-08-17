//Opencleaner - Precision Roller Guide
//Revised 5 Jul 2015
//Designed for 1/4" shaft
//in milimeters, inches are preferred

$fn=1000;

module roller() {

	difference() 
	{
		union() 
		color("black")
		{
		cylinder(h=20.828, d=31.75, $fs=.001); // .820" total width (height) + 1.25" diameter .80" Inner width + 
		cylinder(h=2.54, d=36.8, $fs=.001); //bottom flange, .1" width, 1.45" outside diameter
		translate([0,0,20.574]) //.810" height to top flange
		cylinder(h=2.54, d=36.83, $fs=.001); //top flange, .1" width, 1.45" outside diameter
		}
		union()
		color("black")
		{	
		cylinder(h=80, d=12.7, center = true, $fs=.001); //shaft dimension, 1/2" inch
		cylinder(h=7.1438, d=19.0246, $fs=.001); //bottom bearing offset, .285" wide, .749" diameter
		cylinder(h=2, d=19.05, center=true, $fs=.001); // overcuts bottom bearing offset
		translate([0,0,13.589]) //.535" height to bearing offset
		cylinder(h=7.1438, d=19.0246, $fs=.001); //top bearing offset, .285" wide, .749" diameter
		translate([0,0,20]) //overcuts top bearing offset
		cylinder(h=7.239, d=19.0246, $fs=.001); //top bearing offset, .285" wide, .749" diameter //7.1438	
		translate([0,0,2.54])
		cylinder(h=0.635, d=32.75, $fs=.01); // bottom bias
		translate([0,0,19.95])
		cylinder(h=0.635, d=32.75, $fs=.01); // top bias
		}
	}
}

roller();

module rollerbias1() {

	difference() 
	{
		union() 
		color("black")
		{
		cylinder(h=20.828, d=29, $fs=.001); // .820" total width 
		}
		union()
		color("black")
		{	
		cylinder(h=80, d=12.7, center = true, $fs=.001); //shaft dimension, 1/2" inch
		cylinder(h=7.239, d=19.0246, $fs=.001); //bottom bearing offset, .285" wide, .749" diameter
		cylinder(h=2, d=19.05, center=true, $fs=.001); // overcuts bottom bearing offset
		translate([0,0,13.589]) //.535" height to bearing offset
		cylinder(h=7.239, d=19.0246, $fs=.001); //top bearing offset, .285" wide, .749" diameter
		translate([0,0,20]) //overcuts top bearing offset
		cylinder(h=7.239, d=19.0246, $fs=.001); //top bearing offset, .285" wide, .749" diameter

		}
	}
}

rollerbias1();


//include radius cuts on top and bottom, .025