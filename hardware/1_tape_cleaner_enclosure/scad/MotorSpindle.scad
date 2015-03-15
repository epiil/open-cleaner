module spindle() //UMatic spindle for the takeup side
{
	difference()
	{
		union()
		{
		color("black") cylinder(h=4.8, d=57.25, $fs=.01);
		color("black") cylinder(h=44, d=7.93, $fs=.01);
		color("black") translate([0,0,44]) sphere(d=7.93, $fs=.01);
		color("black") translate([0,0,-9]) cylinder(h=9, d=19.31, $fs=.01);
		color("black") rotate([0,0,120]) cube([1.4,10.73,21], center=true);
		color("black") rotate([0,0,240]) cube([1.4,10.73,21], center=true);
		color("black") rotate([0,0,360]) cube([1.4,10.73,21], center=true);
		color("silver") translate([0,0,-19.31]) cylinder(h=10, d=15.91, $fs=.01);
			{

			}
		}
		
		union()
		color("black")
		{
		translate([-1,3.9,0]) 
		cube([1.4,7,10.8]);
	     	translate([0,3.9,0]) 
		cube([1.4,7,10.8]);
		translate([3.9,-1,0]) 
		rotate([0,0,240]) cube([5,7,10.8]);
		translate([-2.85,-2.85,0]) 
		rotate([0,0,120]) cube([5,7,10.8]);
		translate([0,0,-20]) cylinder(h=50, d=4, $fs=.01);
			{
			}
		}
	}
}



spindle();

module spindle2() //UMatic spindle for the takeup side
{
	difference()
	{
		union()
		{
		color("black") cylinder(h=4.8, d=57.25, $fs=.01);
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



spindle2();
