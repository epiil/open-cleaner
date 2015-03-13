module top()
{
	difference() 
	{
		union() 
		{
			color("grey")
			square(size=[88.138, 406.4], center = true, $fs=.01);
		}
		union() 
		{
			square(size=[20, 70], center = true, $fs=.01);
		}
	}
}

top();