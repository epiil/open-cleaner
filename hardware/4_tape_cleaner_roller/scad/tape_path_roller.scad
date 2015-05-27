module roller() {

	difference() 
	{
		union() 
		color("black")
		{
		cylinder(h=38, d=23, $fs=.01); //supply corner roller
		}
		union()
		color("black")
		{	
		cylinder(h=80, d=6.35, center = true, $fs=.01);
		cylinder(h=10, d=11, center = true, $fs=.01);
		translate([0,0,38]) 
		cylinder(h=10, d=11, center = true, $fs=.01);
		}
	}
}

roller();