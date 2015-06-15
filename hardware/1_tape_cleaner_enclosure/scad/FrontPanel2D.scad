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
			circle(d=2.26, center = true, $fs=.01); //bottom-left LCD hole
			translate([-37.47,15.36])
			circle(d=2.26, center = true, $fs=.01); //bottom-right LCD hole
			translate([37.47,-15.36])
			circle(d=2.26, center = true, $fs=.01); //top-left LCD hole
			translate([-37.47,-15.36])
			circle(d=2.26, center = true, $fs=.01); //top-right LCD hole
			translate([-37.5,25])
			circle(d=6, center = true, $fs=.01); //keypad hole 1
			translate([-20,25])
			circle(d=6, center = true, $fs=.01); //keypad hole 2
			translate([-10,25])
			circle(d=6, center = true, $fs=.01); //keypad hole 3
			translate([-29,20])
			circle(d=6, center = true, $fs=.01); //keypad hole 4
			translate([-29,30])
			circle(d=6, center = true, $fs=.01); //keypad hole 5

		}
	}
}

frontpanelflat();