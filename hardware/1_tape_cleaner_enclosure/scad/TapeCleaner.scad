// tape cleaner enclosure top
// Erik Piil
//piil.erik@gmail.com
//https://github.com/epiil/tape-cleaner
//v20150310

//OUTER DIMENSIONS in mm

outerWidth=390.144;
outerLength=406.4;
outerHeight=2.286;

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
shaftteethNum = 3; //number of teeth 
addendum = 3/2;
dedendum = 3/2;
shafttoothWidth = 3/2;

include<Arduino.scad>


module top()
{
	difference() 
	{
		union() 
		{
			color([.5,.5,.5,1])
			cube(size=[outerWidth, outerLength, outerHeight], center = true, $fs=.01);
		}
		union() 
		{
			color([.5,.5,.5,.6])
			translate([114.3,114.3,0])
			cylinder(h=5, d=61, center = true, $fs=.01); //supply motor spindle cutout
             	translate([-114.3,-114.3,0])
			cylinder(h=5, d=61, center = true, $fs=.01); //takeup motor supply cutout
             	translate([-136,152.4,0]) 
			cylinder(h=5, d=6.35, center = true, $fs=.01); //supply corner roller
             	translate([136,-152.4,0])
			cylinder(h=5, d=6.35, center = true, $fs=.01); // takeup corner roller
             	translate([-69, 77.7,0])  
			rotate(135) cube([6.35,20,5], center = true, $fs=.01); //cleaning path guide roller A
             	translate([69,-77.7,0])
			rotate(135) cube([6.35,20,5], center = true, $fs=.01); //cleaning path guide roller B
             	translate([-18.2, 26.9,0])
			rotate(135) cube([6.35,20,5], center = true, $fs=.01); //cleaning path guide roller C
             	translate([18.2, -26.9,0]) 
			rotate(135) cube([6.35,20,5], center = true, $fs=.01); //cleaning path guide roller D
             	translate([-7,92,0]) 
			cylinder(h=5, d=6.35, center = true, $fs=.01); //cleaning path guide roller E
             	translate([7,-92,0])
			cylinder(h=5, d=6.35, center = true, $fs=.01); //cleaning path guide roller F
             	translate([-30,65,0])  
			cylinder(h=5, d=12.7, center = true, $fs=.01); //cleaning path guide roller G //supply-side supply pellon
             	translate([30,-65,0])
			cylinder(h=5, d=12.7, center = true, $fs=.01); //cleaning path guide roller H //takeup-side supply pellon
             	translate([-50.8,108,0]) 
			cylinder(h=5, d=12.7, center = true, $fs=.01); //cleaning path guide roller I //supply-side takeup pellon
             	translate([50.8,-108,0])
			cylinder(h=5, d=12.7, center = true, $fs=.01); //cleaning path guide roller J //takeup-side takeup pellon
            	translate([-80.7, 152.4,0]) 
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


module spindlesupply() //UMatic spindle for the takeup side
{
	difference()
	{
		union()
		{
		translate([-114.3,-114.3,0])
		color("black") cylinder(h=4.8, d=57.25, $fs=.01);
		translate([-114.3,-114.3,0])
		color("black") cylinder(h=44, d=7.93, $fs=.01);
		translate([-114.3,-114.3,44])
		color("black") sphere(d=7.93, $fs=.01);
		translate([-114.3,-114.3,-9])
		color("black") cylinder(h=9, d=19.31, $fs=.01);
		translate([-114.3,-114.3,0])
		color("black") rotate([0,0,120]) cube([1.4,10.73,21], center=true);
		translate([-114.3,-114.3,0])
		color("black") rotate([0,0,240]) cube([1.4,10.73,21], center=true);
		translate([-114.3,-114.3,0])
		color("black") rotate([0,0,360]) cube([1.4,10.73,21], center=true);
		translate([-114.3,-114.3,-19.31])
		color("silver") cylinder(h=10, d=15.91, $fs=.01);
			{

			}
		}
		
		union()
		color("black")
		{
		translate([-113.3,-118.2,0])
		cube([1.4,7,10.8]);
		translate([-114.3,-118.2,0])
		cube([1.4,7,10.8]);
		translate([-118.2,-113.3,0])
		rotate([0,0,240]) cube([5,7,10.8]);
		translate([-111.45,-111.45,0]) 
		rotate([0,0,120]) cube([5,7,10.8]);
		translate([-114.3,-114.3,-20])
		cylinder(h=50, d=4, $fs=.01);
			{
			}
		}
	}
}



spindlesupply();

module spindle2supply() //UMatic spindle for the takeup side
{
	difference()
	{
		union()
		{
		translate([-114.3,-114.3,0])
		color("black") cylinder(h=4.8, d=57.25, $fs=.01);
		translate([-114.3,-114.3,0])
		color("black") cylinder(h=44, d=7.93, $fs=.01);
			{

			}
		}
		
		union()
		{
			{
			}
		}
	}
}



spindle2supply();




module spindletakeup() //UMatic spindle for the takeup side
{
	difference()
	{
		union()
		{
		translate([114.3,114.3,0])
		color("black") cylinder(h=4.8, d=57.25, $fs=.01);
		translate([114.3,114.3,0])
		color("black") cylinder(h=44, d=7.93, $fs=.01);
		translate([114.3,114.3,44])
		color("black") sphere(d=7.93, $fs=.01);
		translate([114.3,114.3,-9])
		color("black") cylinder(h=9, d=19.31, $fs=.01);
		translate([114.3,114.3,0])
		color("black") rotate([0,0,120]) cube([1.4,10.73,21], center=true);
		translate([114.3,114.3,0])
		color("black") rotate([0,0,240]) cube([1.4,10.73,21], center=true);
		translate([114.3,114.3,0])
		color("black") rotate([0,0,360]) cube([1.4,10.73,21], center=true);
		translate([114.3,114.3,-19.31])
		color("silver") cylinder(h=10, d=15.91, $fs=.01);
			{

			}
		}
		
		union()
		color("black")
		{
		translate([113.3,118.2,0])
		cube([1.4,7,10.8]);
		translate([114.3,118.2,0])
		cube([1.4,7,10.8]);
		translate([118.2,113.3,0])
		rotate([0,0,240]) cube([5,7,10.8]);
		translate([111.45,111.45,0]) 
		rotate([0,0,120]) cube([5,7,10.8]);
		translate([114.3,114.3,-20])
		cylinder(h=50, d=4, $fs=.01);
			{
			}
		}
	}
}



spindletakeup();

module spindle2takeup() //UMatic spindle for the takeup side
{
	difference()
	{
		union()
		{
		translate([114.3,114.3,0])
		color("black") cylinder(h=4.8, d=57.25, $fs=.01);
		translate([114.3,114.3,0])
		color("black") cylinder(h=44, d=7.93, $fs=.01);
			{

			}
		}
		
		union()
		{
			{
			}
		}
	}
}



spindle2takeup();

module guiderollers() //guide rollers 
{
	difference() 
	{
		union() 
		color("silver")
		{
             	translate([-136,152.4,0])
			cylinder(h=38, d=23, $fs=.01); //supply corner roller 
             	translate([136,-152.4,0])
			cylinder(h=38, d=23, $fs=.01); // takeup corner roller
           		translate([-25.4, 152.4,0]) 
			cylinder(h=5, d=6.35, center = true, $fs=.01); //supplyside initial roller
             	translate([-69, 77.7,0])  
			cylinder(h=38, d=12.6, $fs=.01); //cleaning path guide roller A
             	translate([69,-77.7,0])
			cylinder(h=38, d=12.6, $fs=.01); //cleaning path guide roller B
             	translate([-18.2, 26.9,0])
			cylinder(h=38, d=12.6, $fs=.01); //cleaning path guide roller C
             	translate([18.2, -26.9,0])
			cylinder(h=38, d=12.6, $fs=.01); //cleaning path guide roller D
             	translate([-7,92,0])
			cylinder(h=38, d=12.6, $fs=.01); //cleaning path guide roller E
             	translate([7,-92,0])
			cylinder(h=38, d=12.6, $fs=.01); //cleaning path guide roller F
             	translate([-30,65,0])
			cylinder(h=38, d=13.5, $fs=.01); //cleaning path guide roller G //supply-side supply pellon
             	translate([30,-65,0])
			cylinder(h=38, d=13.5, $fs=.01); //cleaning path guide roller H //takeup-side supply pellon
             	translate([-50.8,108,0])
			cylinder(h=38, d=13.5, $fs=.01); //cleaning path guide roller I //supply-side takeup pellon
             	translate([50.8,-108,0])
			cylinder(h=38, d=13.5, $fs=.01); //cleaning path guide roller J //takeup-side takeup pellon
            	translate([-80.7, 152.4,0])
			cylinder(h=38, d=23, $fs=.01); //tension arm roller 
             	translate([-25.4, 152.4,0]) 
			cylinder(h=38, d=23, $fs=.01); //supplyside initial roller
		}
		union()
        color("silver")
		{
             	translate([-136,152.4,0])
			cylinder(h=80, d=6.35, center = true, $fs=.01); //supply corner roller 
             	translate([136,-152.4,0])
			cylinder(h=80, d=6.35, center = true, $fs=.01); // takeup corner roller
             	translate([-69, 77.7,0])
			cylinder(h=80, d=6.35, center = true, $fs=.01); //cleaning path guide roller A
             	translate([69,-77.7,0])
			cylinder(h=80, d=6.35, center = true, $fs=.01); //cleaning path guide roller B
             	translate([-18.2, 26.9,0])
			cylinder(h=80, d=6.35, center = true, $fs=.01); //cleaning path guide roller C
             	translate([18.2, -26.9,0])
			cylinder(h=80, d=6.35, center = true, $fs=.01); //cleaning path guide roller D
             	translate([-7,92,0])
			cylinder(h=80, d=6.35, center = true, $fs=.01); //cleaning path guide roller E
             	translate([7,-92,0])
			cylinder(h=80, d=6.35, center = true, $fs=.01); //cleaning path guide roller F
             	translate([-30,65,0])
			cylinder(h=80, d=6.35, center = true, $fs=.01); //cleaning path guide roller G //supply-side supply pellon
             	translate([30,-65,0])
			cylinder(h=80, d=6.35, center = true, $fs=.01); //cleaning path guide roller H //takeup-side supply pellon
             	translate([-50.8,108,0])
			cylinder(h=80, d=6.35, center = true, $fs=.01); //cleaning path guide roller I //supply-side takeup pellon
             	translate([50.8,-108,0])
			cylinder(h=80, d=6.35, center = true, $fs=.01); //cleaning path guide roller J //takeup-side takeup pellon
            	translate([-80.7, 152.4,0])
			cylinder(h=80, d=6.35, center = true, $fs=.01); //tension arm roller
			translate([-25.4, 152.4,0]) 
			cylinder(h=80, d=6.35, center = true, $fs=.01); //supplyside initial roller
		}
	}
}

guiderollers();

module guiderollerscrews()
{
	difference() 
	{
		union() 
		color("silver")
		{
			translate([114.3,114.3,16])
			cylinder(h=49.30, d=6.35, center = true, $fs=.01); //supply motor spindle cutout
             	translate([-114.3,-114.3,16])
			cylinder(h=49.30, d=6.35, center = true, $fs=.01); //takeup motor supply cutout
			translate([-136,152.4,16])
			cylinder(h=49.30, d=6.35, center = true, $fs=.01); //supply corner roller 
			translate([136,-152.4,16])
			cylinder(h=49.30, d=6.35, center = true, $fs=.01); // takeup corner roller
             	translate([-69, 77.7,16])
			cylinder(h=49.30, d=6.35, center = true, $fs=.01); //cleaning path guide roller A
             	translate([69,-77.7,16])
			cylinder(h=49.30, d=6.35, center = true, $fs=.01); //cleaning path guide roller B
             	translate([-18.2, 26.9,16])
			cylinder(h=49.30, d=6.35, center = true, $fs=.01); //cleaning path guide roller C
             	translate([18.2, -26.9,16])
			cylinder(h=49.30, d=6.35, center = true, $fs=.01); //cleaning path guide roller D
             	translate([-7,92,16])
			cylinder(h=49.30, d=6.35, center = true, $fs=.01); //cleaning path guide roller E
             	translate([7,-92,16])
			cylinder(h=49.30, d=6.35, center = true, $fs=.01); //cleaning path guide roller F
             	translate([-30,65,16])
			cylinder(h=49.30, d=6.35, center = true, $fs=.01); //cleaning path guide roller G //supply-side supply pellon
             	translate([30,-65,16])
			cylinder(h=49.30, d=6.35, center = true, $fs=.01); //cleaning path guide roller H //takeup-side supply pellon
             	translate([-50.8,108,16])
			cylinder(h=49.30, d=6.35, center = true, $fs=.01); //cleaning path guide roller I //supply-side takeup pellon
             	translate([50.8,-108,16])
			cylinder(h=49.30, d=6.35, center = true, $fs=.01); //cleaning path guide roller J //takeup-side takeup pellon
            	translate([-80.7, 152.4,16])
			cylinder(h=49.30, d=6.35, center = true, $fs=.01); //tension arm roller
             	translate([-25.4, 152.4,16]) 
			cylinder(h=49.30, d=6.35, center = true, $fs=.01); //supplyside initial roller
		}
		union()

		{

		}
	}
}

guiderollerscrews();

module pellonrolls()
{
	difference() 
	{
		union() 
		color("blue")
		{
			translate([-50.8,108,0]) 
			cylinder(h=38, d=40, $fs=.01); //supply-side takeup pellon
             	translate([50.8,-108,0])
			cylinder(h=38, d=40, $fs=.01); //takeup-side takeup pellon
			translate([-30,65,0])
			cylinder(h=38, d=40, $fs=.01); //supply-side supply pellon
             	translate([30,-65,0])
			cylinder(h=38, d=40, $fs=.01); //takeup-side supply pellon
			translate([12,-25,0])
			rotate(259) cube([68,1,38], $fs=.01); 
			translate([-24,26,0])
			rotate(126) cube([30,1,38], $fs=.01);
			translate([24,-26,0])
			rotate(308) cube([30,1,38], $fs=.01); 
			translate([-12,25,0])
			rotate(80) cube([70,1,38], $fs=.01); 
			translate([-9,99,0])
			rotate(204) cube([60,1,38], $fs=.01); 
			translate([9,-99,0])
			rotate(385) cube([60,1,38], $fs=.01); 
			translate([-75,80,0])
			rotate(79) cube([35,1,38], $fs=.01); 
			translate([75,-80,0])
			rotate(260) cube([35,1,38], $fs=.01); 
		}
		union()
        color("white")
		{
			translate([-101.6,50.8,0])
			cylinder(h=38, d=13.5, $fs=.01);
             	translate([101.6,-50.8,0])
			cylinder(h=38, d=13.5, $fs=.01);
			translate([-70,25,0])
			cylinder(h=38, d=13.5, $fs=.01);
             	translate([70,-25,0])
			cylinder(h=38, d=13.5, $fs=.01);
		}
	}
}

pellonrolls();


module irsensors()
{
	difference() 
	{
		union() 
		color("black")
		{
	        	translate([-40.4,-13.4,0])
			rotate(315) cube(size=[40, 10.5, 15], $fs=.01); //IR sensor takeup side
	        	translate([13.4, 40.4,0])
			rotate(315) cube(size=[40, 10.5, 15], $fs=.01); //IR sensor supply side
	        	translate([-127, 127,0])
			rotate(45) cube(size=[20, 5.1, 30], center = true, $fs=.01); //photoop sensor takeup side
	        	translate([127, -127,0])
			rotate(45) cube(size=[20, 5.1, 30], center = true, $fs=.01); //photoop sensor supply side
		}
		union()
		color("black")
		{
	        	translate([-127, 127,0])
			rotate(45) cube(size=[10, 7, 40], center = true, $fs=.01); //photoop sensor takeup side
	        	translate([127, -127,0])
			rotate(45) cube(size=[10, 7, 40], center = true, $fs=.01); //photoop sensor supply side
		}
	}
}

irsensors();

module bottom()
{
	difference() 
	{
		union() 
		color("grey")
		{
			translate([0,0,-88.138]) cube(size=[outerWidth, outerLength, outerHeight], center = true, $fs=.01);
			translate([90,65,-88.138]) cube(size=[5, 75, 65], $fs=.01);
			translate([-90,-140,-88.138]) cube(size=[5, 75, 65], $fs=.01);
			translate([60,-125,-88.138]) cube(size=[5, 35, 65], $fs=.01);
			translate([-30, 90,-88.138]) cube(size=[5, 35, 65], $fs=.01);
		}
		union() 
		{

		}
	}
}

bottom();

module spindlemotors()
{
	difference() 
	{
		union() 
		{
			color("gold")
			translate([114.3,114.3,-89])
			cylinder(h=70, d=40.68, $fs=.01); //supply motor spindle cutout
             	translate([-114.3,-114.3,-89])
			cylinder(h=70, d=40.68, $fs=.01); //takeup motor supply cutout
		}
		union() 
		{

		}
	}
}

spindlemotors();

module pellonmotors()
{
	difference() 
	{
		union() 
		{
			color("gold")
			translate([-50.8,108,-89])
			cylinder(h=85, d=25, $fs=.01); 
                 translate([50.8,-108,-89])
			cylinder(h=85, d=25, $fs=.01); 
		}
		union() 
		{

		}
	}
}

pellonmotors();

module frontpanel()
{
	difference() 
	{
		union() 
		{
			color("grey")
			translate([0,205,-44])
			cube(size=[406.4, 3, 88.138], center = true, $fs=.01);
		}
		union() 
		{
			translate([0,205,-44])
			cube(size=[71, 5, 26.3], center = true, $fs=.01);
			translate([170,205,-44])
			cube(size=[12.5, 5, 17.2], center = true, $fs=.01);
			translate([36.195,205,-29.695])
			sphere(d=5, center = true, $fs=.01);
			translate([-36.195,205,-29.695])
			sphere(d=5, center = true, $fs=.01);
			translate([36.195,205,-58.305])
			sphere(d=5, center = true, $fs=.01);
			translate([-36.195,205,-58.305])
			sphere(d=5, center = true, $fs=.01);

		}
	}
}

frontpanel();

module backpanel()
{
	difference() 
	{
		union() 
		{
			color("grey")
			translate([0,-205,-44])
			cube(size=[406.4, 3, 88.138], center = true, $fs=.01);
		}
		union() 
		{

		}
	}
}

backpanel();

//module sideleftpanel()
//{
//	difference() 
//	{
//		union() 
//		{
//			color("grey")
//			translate([205,0,-44])
//			rotate(90) cube(size=[406.4, 3, 88.138], center = true, $fs=.01);
//		}
//		union() 
//		{
//
//		}
//	}
//}

//sideleftpanel();

//module siderightpanel()
//{
//	difference() 
//	{
//		union() 
//		{
//			color("grey")
//			translate([-205,0,-44])
//			rotate(90) cube(size=[406.4, 3, 88.138], center = true, $fs=.01);
//		}
//		union() 
//		{
//
//		}
//	}
//}

//siderightpanel();

module LED()
{
	difference() 
	{
		union() 
		{
			color("black")
			translate([0,205,-44])
			cube(size=[71, 5, 26.3], center = true, $fs=.01);
			color("red")
			translate([0,203,-44])
			cube(size=[80.07, 2, 36.07], center = true, $fs=.01);
		}
		union() 
		{
            	translate([36.195,203,-29.695])
			sphere(d=5, center = true, $fs=.01);
			translate([-36.195,203,-29.695])
			sphere(d=5, center = true, $fs=.01);
			translate([36.195,203,-58.305])
			sphere(d=5, center = true, $fs=.01);
			translate([-36.195,203,-58.305])
			sphere(d=5, center = true, $fs=.01);
		}
	}
}

LED();

module arduino() //Arduino and motor shield
{
	difference()
	{
		union()

		{
			translate([40, 110, -78]) rotate(180) Arduino(1, 0, 0);
			color("tan") translate([40, 110, -60]) rotate(180) Arduino(1, 1, 1);
			{

			}
		}
		
		union()

		{

			{
			}
		}
	}
}



arduino(); 

module onoffswitch()
{
	difference() 
	{
		union() 
		color("black")
		{
			translate([170,202,-44])
			cube(size=[12.5, 15, 17.2], center = true, $fs=.01);
		}
		union() 
		{

		}
	}
}

onoffswitch();

module servos()
{
	difference() 
	{
		union() 
		color("black")
		{
			translate([114.3,75,-89])
			cube([40,19.5,70], $fs=.01); 
                 translate([-130,-93,-89])
			cube([40,19.5,70], $fs=.01); 
		}
		union() 
		{

		}
	}
}

servos();

module tensionarm()
{
	difference() 
	{
		union() 
		color("silver")
		{
            	translate([-75, 120,-89]) 
		 	rotate(90) cube(size=[60, 20, 85], $fs=.01);
		}
		union() 
		{

		}
	}
}

tensionarm();

//module tapepathreference()
//{
//	difference() 
//	{
//		union() 
//		color("black")
//		{
//
//			translate([0,0,10])
//			rotate(135) cube([470,.5,14], center=true, $fs=.01); 
//		}
//		union()
//	      color("white")
//		{
//
//		}
//	}
//}

//tapepathreference();










