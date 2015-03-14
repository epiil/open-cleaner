module sideleftpanel()
{
	difference() 
	{
		union() 
		{
			color("grey")
			translate([205,0,-44])
			rotate(90) cube(size=[406.4, 3, 88.138], center = true, $fs=.01);
		}
		union() 
		{

		}
	}
}

sideleftpanel();
