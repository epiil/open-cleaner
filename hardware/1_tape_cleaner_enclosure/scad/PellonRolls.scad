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