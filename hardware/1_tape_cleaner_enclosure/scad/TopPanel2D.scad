// tape cleaner enclosure top
// Erik Piil
//piil.erik@gmail.com
//https://github.com/epiil/tape-cleaner
//v20150310

//OUTER DIMENSIONS in mm

outerWidth=390.144;
outerLength=406.4;

module top()
{
	difference() 
	{
		union() 
		{
			color([.5,.5,.5,1])
			square(size=[outerWidth, outerLength], center = true, $fs=.01);
		}
		union() 
		{
			color([.5,.5,.5,.6])
			translate([114.3,114.3])
			circle(h=5, d=61, center = true, $fs=.01); //supply motor spindle cutout
             	translate([-114.3,-114.3])
			circle(h=5, d=61, center = true, $fs=.01); //takeup motor supply cutout
             	translate([-136,152.4]) 
			circle(h=5, d=6.45, center = true, $fs=.01); //supply corner roller
             	translate([136,-152.4])
			circle(h=5, d=6.45, center = true, $fs=.01); // takeup corner roller
             	translate([-69, 77.7])  
			rotate(135) square([6.45,20], center = true, $fs=.01); //cleaning path guide roller A
             	translate([69,-77.7])
			rotate(135) square([6.45,20], center = true, $fs=.01); //cleaning path guide roller B
             	translate([-18.2, 26.9])
			rotate(135) square([6.45,20], center = true, $fs=.01); //cleaning path guide roller C
             	translate([18.2, -26.9]) 
			rotate(135) square([6.45,20], center = true, $fs=.01); //cleaning path guide roller D
             	translate([-7,92]) 
			circle(h=5, d=6.45, center = true, $fs=.01); //cleaning path guide roller E
             	translate([7,-92])
			circle(h=5, d=6.45, center = true, $fs=.01); //cleaning path guide roller F
             	translate([-30,65])  
			circle(h=5, d=12.7, center = true, $fs=.01); //cleaning path guide roller G //supply-side supply pellon
             	translate([30,-65])
			circle(h=5, d=12.7, center = true, $fs=.01); //cleaning path guide roller H //takeup-side supply pellon
             	translate([-50.8,108]) 
			circle(h=5, d=12.7, center = true, $fs=.01); //cleaning path guide roller I //supply-side takeup pellon
             	translate([50.8,-108])
			circle(h=5, d=12.7, center = true, $fs=.01); //cleaning path guide roller J //takeup-side takeup pellon
            	translate([-80.7, 152.4]) 
			rotate(90) square(size=[50.8, 10.5], center = true, $fs=.01); //tension arm roller cutout
           		translate([-25.4, 152.4]) 
			circle(h=5, d=6.45, center = true, $fs=.01); //supplyside initial roller
	        	translate([-25.4,-25.4])
			rotate(45) square(size=[40, 10.5], center = true, $fs=.01); //IR sensor takeup side
	        	translate([25.4, 25.4])
			rotate(45) square(size=[40, 10.5], center = true, $fs=.01); //IR sensor supply side
	        	translate([-127, 127])
			rotate(45) square(size=[35, 5.1], center = true, $fs=.01); //photoop sensor takeup side
	        	translate([127, -127])
			rotate(45) square(size=[35, 5.1], center = true, $fs=.01); //photoop sensor supply side
		}
	}
}

top();

