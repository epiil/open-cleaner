module pellonrolls()
{
	difference() 
	{
		union() 
		color("white")
		{
			translate([-101.6,50.8,0])
			cylinder(h=38, d=40, $fs=.01);
             translate([101.6,-50.8,0])
			cylinder(h=38, d=40, $fs=.01);
			  translate([-70,25,0])
			cylinder(h=38, d=40, $fs=.01);
             translate([70,-25,0])
			cylinder(h=38, d=40, $fs=.01);
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
