module frontpanelflat()
{
	difference() 
	{
		union() 
		{
			color("blue")
			square([406.4,88.138],center = true);
		}
		union() 
		{
			square([71.48,26.36], center=true);
			translate([170,0])
			square([12.5,17.2], center = true);
			translate([37.47,15.36])
			circle(d=2.26, center = true, $fs=.01);
			translate([-37.47,15.36])
			circle(d=2.26, center = true, $fs=.01);
			translate([37.47,-15.36])
			circle(d=2.26, center = true, $fs=.01);
			translate([-37.47,-15.36])
			circle(d=2.26, center = true, $fs=.01);

		}
	}
}

frontpanelflat();