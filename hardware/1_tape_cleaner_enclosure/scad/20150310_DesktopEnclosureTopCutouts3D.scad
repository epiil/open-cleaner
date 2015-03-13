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
			color("dark grey")
			cube(size=[outerWidth, outerLength, outerHeight], center = true, $fs=.01);
		}
		union() 
		{
			translate([114.3,114.3,0])
			cylinder(h=5, d=25.4, center = true, $fs=.01); //supply motor spindle cutout
             translate([-114.3,-114.3,0])
			cylinder(h=5, d=25.4, center = true, $fs=.01); //takeup motor supply cutout
             translate([-141.2875,152.4,0]) 
			cylinder(h=5, d=6.35, center = true, $fs=.01); //supply corner roller
             translate([141.2875,-152.4,0])
			cylinder(h=5, d=6.35, center = true, $fs=.01); // takeup corner roller
             translate([-69.85, 76.2,0])  
			cylinder(h=5, d=6.35, center = true, $fs=.01); //cleaning path guide roller A
             translate([69.85,-76.2,0])
			cylinder(h=5, d=6.35, center = true, $fs=.01); //cleaning path guide roller B
             translate([-19.05, 25.4,0])
			cylinder(h=5, d=6.35, center = true, $fs=.01); //cleaning path guide roller C
             translate([19.05, -25.4,0]) 
			cylinder(h=5, d=6.35, center = true, $fs=.01); //cleaning path guide roller D
             translate([0,88.9,0]) 
			cylinder(h=5, d=6.35, center = true, $fs=.01); //cleaning path guide roller E
             translate([0, -88.9,0])
			cylinder(h=5, d=6.35, center = true, $fs=.01); //cleaning path guide roller F
             translate([-30,65,0])  
			cylinder(h=5, d=6.35, center = true, $fs=.01); //cleaning path guide roller G //supply-side supply pellon
             translate([30,-65,0])
			cylinder(h=5, d=6.35, center = true, $fs=.01); //cleaning path guide roller H //takeup-side supply pellon
             translate([-50.8,108,0]) 
			cylinder(h=5, d=12.7, center = true, $fs=.01); //cleaning path guide roller I //supply-side takeup pellon
             translate([50.8,-108,0])
			cylinder(h=5, d=12.7, center = true, $fs=.01); //cleaning path guide roller J //takeup-side takeup pellon
            translate([-83.3, 152.4,0]) 
			rotate(90) cube(size=[50.8, 10.5, 5], center = true, $fs=.01); //tension arm roller cutout
           translate([-25.4, 152.4,0]) 
			cylinder(h=5, d=6.35, center = true, $fs=.01); //supplyside initial roller
	        translate([-25.4,-25.4,0])
			rotate(45) cube(size=[40, 10.5, 5], center = true, $fs=.01); //IR sensor takeup side
	        translate([25.4, 25.4,0])
			rotate(45) cube(size=[40, 10.5, 5], center = true, $fs=.01); //IR sensor supply side
	        translate([-127, 127,0])
			rotate(45) cube(size=[35, 5.1, 5], center = true, $fs=.01); //photoop sensor takeup side
	        translate([127, -127,0])
			rotate(45) cube(size=[35, 5.1, 5], center = true, $fs=.01); //photoop sensor supply side
		}
	}
}

top();

