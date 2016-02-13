//Opencleaner - Pellon Mount
//Revised 19 August 2015
//Designed for 1/4" shaft
//in milimeters; inches are preferred

$fn=1000;

module roller() {

	difference() 
	{
		union() 
		color("black")
		{
		cylinder(h=36.30, d=15.43, $fs=.001); // .820" total width (height) + 1.25" diameter .80" Inner width + 
		cylinder(h=2.00, d=38.1, $fs=.001); //bottom flange, .1" width, 1.45" outside diameter
		}
		union()
		color("black")
		{	
		cylinder(h=80, d=6.35, center = true, $fs=.001); //shaft dimension, 1/4" inch
		}
	}
}

roller();
