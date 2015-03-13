// tape cleaner enclosure top
// Erik Piil
//piil.erik@gmail.com
//https://github.com/epiil/tape-cleaner
//v20150310

//OUTER DIMENSIONS in mm

outerWidth=406.4;
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

module spindlesupply() //UMatic spindle for the supply side
{
	difference()
	{
		union()
		color("black")
		{
			translate([-114.3,-114.3,0])
			cylinder(r=outerRadius, h=outerHeight, $fs=.01);
			translate([-114.3,-114.3,0])
			cylinder(d=innerDiam, h=innerHeight, $fs=.01);
			translate([-123.3,-114,0])
			cylinder(h=innerHeight+prongHeight, r=prongRadius);
			translate([-110.3,-122,0])
			cylinder(h=innerHeight+prongHeight, r=prongRadius);
			translate([-109.3,-106,0])
			cylinder(h=innerHeight+prongHeight, r=prongRadius);
			{

			}
		}
		
		union()
		color("black")
		{
			translate([-114.3,-114.3,0])
			cylinder(r=shaftRadius, h=innerHeight*2+1, center=true);
			translate([0,0,10/2]) spur(3, 8, 10);
			if (centerShape==1) solid(10, 8);	
			{
			}
		}
	}
}



spindlesupply(); 

module spindletakeup() //UMatic spindle for the takeup side
{
	difference()
	{
		union()
		color("black")
		{
			translate([114.3,114.3,0])
			cylinder(r=outerRadius, h=outerHeight, $fs=.01);
			translate([114.3,114.3,0])
			cylinder(d=innerDiam, h=innerHeight, $fs=.01);
			translate([123.3,114,0])
			cylinder(h=innerHeight+prongHeight, r=prongRadius);
			translate([110.3,122,0])
			cylinder(h=innerHeight+prongHeight, r=prongRadius);
			translate([109.3,106,0])
			cylinder(h=innerHeight+prongHeight, r=prongRadius);
			{

			}
		}
		
		union()
		color("black")
		{
			translate([114.3,114.3,0])
			cylinder(r=shaftRadius, h=innerHeight*2+1, center=true);
			translate([0,0,10/2]) spur(3, 8, 10);
			if (centerShape==1) solid(10, 8);	
			{
			}
		}
	}
}



spindletakeup();

module guiderollers() //guide rollers 
{
	difference() 
	{
		union() 
		color("silver")
		{
             translate([-141.2875,152.4,0])
			cylinder(h=38, d=23, $fs=.01); //supply corner roller 
             translate([141.2875,-152.4,0])
			cylinder(h=38, d=23, $fs=.01); // takeup corner roller
           translate([-25.4, 152.4,0]) 
			cylinder(h=5, d=6.35, center = true, $fs=.01); //supplyside initial roller
             translate([-69.85, 76.2,0])
			cylinder(h=38, d=12.6, $fs=.01); //cleaning path guide roller A
             translate([69.85,-76.2,0])
			cylinder(h=38, d=12.6, $fs=.01); //cleaning path guide roller B
             translate([-19.05, 25.4,0])
			cylinder(h=38, d=12.6, $fs=.01); //cleaning path guide roller C
             translate([19.05, -25.4,0])
			cylinder(h=38, d=12.6, $fs=.01); //cleaning path guide roller D
             translate([0,88.9,0])
			cylinder(h=38, d=12.6, $fs=.01); //cleaning path guide roller E
             translate([0, -88.9,0])
			cylinder(h=38, d=12.6, $fs=.01); //cleaning path guide roller F
             translate([-30,65,0])
			cylinder(h=38, d=13.5, $fs=.01); //cleaning path guide roller G //supply-side supply pellon
             translate([30,-65,0])
			cylinder(h=38, d=13.5, $fs=.01); //cleaning path guide roller H //takeup-side supply pellon
             translate([-50.8,108,0])
			cylinder(h=38, d=13.5, $fs=.01); //cleaning path guide roller I //supply-side takeup pellon
             translate([50.8,-108,0])
			cylinder(h=38, d=13.5, $fs=.01); //cleaning path guide roller J //takeup-side takeup pellon
            translate([-83.3, 152.4,0])
			cylinder(h=38, d=23, $fs=.01); //tension arm roller 
             translate([-25.4, 152.4,0]) 
			cylinder(h=38, d=23, $fs=.01); //supplyside initial roller
		}
		union()
        color("silver")
		{
             translate([-141.2875,152.4,0])
			cylinder(h=80, d=6.35, center = true, $fs=.01); //supply corner roller 
             translate([141.2875,-152.4,0])
			cylinder(h=80, d=6.35, center = true, $fs=.01); // takeup corner roller
             translate([-69.85, 76.2,0])
			cylinder(h=80, d=6.35, center = true, $fs=.01); //cleaning path guide roller A
             translate([69.85,-76.2,0])
			cylinder(h=80, d=6.35, center = true, $fs=.01); //cleaning path guide roller B
             translate([-19.05,25.4,0])
			cylinder(h=80, d=6.35, center = true, $fs=.01); //cleaning path guide roller C
             translate([19.05, -25.4,0])
			cylinder(h=80, d=6.35, center = true, $fs=.01); //cleaning path guide roller D
             translate([0,88.9,0])
			cylinder(h=80, d=6.35, center = true, $fs=.01); //cleaning path guide roller E
             translate([0, -88.9,0])
			cylinder(h=80, d=6.35, center = true, $fs=.01); //cleaning path guide roller F
             translate([-30,65,0])
			cylinder(h=80, d=6.35, center = true, $fs=.01); //cleaning path guide roller G //supply-side supply pellon
             translate([30,-65,0])
			cylinder(h=80, d=6.35, center = true, $fs=.01); //cleaning path guide roller H //takeup-side supply pellon
             translate([-50.8,108,0])
			cylinder(h=80, d=6.35, center = true, $fs=.01); //cleaning path guide roller I //supply-side takeup pellon
             translate([50.8,-108,0])
			cylinder(h=80, d=6.35, center = true, $fs=.01); //cleaning path guide roller J //takeup-side takeup pellon
            translate([-83.3, 152.4,0])
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
			translate([114.3,114.3,0])
			cylinder(h=82, d=6.35, center = true, $fs=.01); //supply motor spindle cutout
             translate([-114.3,-114.3,0])
			cylinder(h=82, d=6.35, center = true, $fs=.01); //takeup motor supply cutout
             translate([-141.2875,152.4,0])
			cylinder(h=80, d=6.35, center = true, $fs=.01); //supply corner roller 
             translate([141.2875,-152.4,0])
			cylinder(h=82, d=6.35, center = true, $fs=.01); // takeup corner roller
             translate([-69.85, 76.2,0])
			cylinder(h=82, d=6.35, center = true, $fs=.01); //cleaning path guide roller A
             translate([69.85,-76.2,0])
			cylinder(h=82, d=6.35, center = true, $fs=.01); //cleaning path guide roller B
             translate([-19.05,25.4,0])
			cylinder(h=82, d=6.35, center = true, $fs=.01); //cleaning path guide roller C
             translate([19.05, -25.4,0])
			cylinder(h=82, d=6.35, center = true, $fs=.01); //cleaning path guide roller D
             translate([0,88.9,0])
			cylinder(h=82, d=6.35, center = true, $fs=.01); //cleaning path guide roller E
             translate([0, -88.9,0])
			cylinder(h=82, d=6.35, center = true, $fs=.01); //cleaning path guide roller F
             translate([-30,65,0])
			cylinder(h=82, d=6.35, center = true, $fs=.01); //cleaning path guide roller G //supply-side supply pellon
             translate([30,-65,0])
			cylinder(h=82, d=6.35, center = true, $fs=.01); //cleaning path guide roller H //takeup-side supply pellon
             translate([-50.8,108,0])
			cylinder(h=82, d=6.35, center = true, $fs=.01); //cleaning path guide roller I //supply-side takeup pellon
             translate([50.8,-108,0])
			cylinder(h=82, d=6.35, center = true, $fs=.01); //cleaning path guide roller J //takeup-side takeup pellon
            translate([-83.3, 152.4,0])
			cylinder(h=82, d=6.35, center = true, $fs=.01); //tension arm roller
             translate([-25.4, 152.4,0]) 
			cylinder(h=82, d=6.35, center = true, $fs=.01); //supplyside initial roller
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
		color("white")
		{
			translate([-50.8,108,0]) 
			cylinder(h=38, d=40, $fs=.01); //supply-side takeup pellon
             translate([50.8,-108,0])
			cylinder(h=38, d=40, $fs=.01); //takeup-side takeup pellon
			  translate([-30,65,0])
			cylinder(h=38, d=40, $fs=.01); //supply-side supply pellon
             translate([30,-65,0])
			cylinder(h=38, d=40, $fs=.01); //takeup-side supply pellon
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
	        translate([-25.4,-25.4,0])
			rotate(315) cube(size=[40, 10.5, 30], center = true, $fs=.01); //IR sensor takeup side
	        translate([25.4, 25.4,0])
			rotate(315) cube(size=[40, 10.5, 30], center = true, $fs=.01); //IR sensor supply side
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



