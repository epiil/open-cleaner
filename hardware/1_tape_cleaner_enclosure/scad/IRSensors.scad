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