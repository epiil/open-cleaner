module backpanelflat()
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
			translate([150,0])
			square([27,20], center = true);

		}
	}
}

backpanelflat();