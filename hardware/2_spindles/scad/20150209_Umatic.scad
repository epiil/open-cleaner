// 3/4" U-Matic tape cleaner spindle
// Erik Piil
//piil.erik@gmail.com
//https://github.com/epiil/tape-cleaner
//v20150208


//SPINDLE "OUTER" PARAMETERS
outerRadius=60; //outer "seat" radius

outerDiam=120; //outer "seat" diameter

outerHeight=2; //outer "seat" height


//SPINDLE "INNER" DIAMETERS
innerRadius=13.5; //inner spindle radius

innerDiam=29.62; //inner spindle diameter

innerHeight=20; //inner spindle height

shaftDiam=8; //inner shaft diameter

shaftRadius=4; //inner shaft radius

//TEETH PARAMETERS

prongNum=3; //number of prongs (int)

prongRadius=1.92;

prongDiam=3.84;

prongHeight=3.93;

module spindle()
{
	difference()
	{
		union()
		{
			cylinder(r=outerRadius, h=outerHeight, $fs=.01);
			cylinder(d=29.86, h=innerHeight, $fs=.01);
			translate([9,0,0])
			cylinder(h=innerHeight+prongHeight, r=prongRadius);
			translate([-4,8,0])
			cylinder(h=innerHeight+prongHeight, r=prongRadius);
			translate([-5,-8,0])
			cylinder(h=innerHeight+prongHeight, r=prongRadius);
			{

			}
		}
		
		difference()
		{
			cylinder(r=shaftRadius, h=innerHeight*2+1, center=true);
			{

			}
		}
	}
}



spindle();
