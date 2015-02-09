
// globals
co_radius = 29.80;
co_height = 19.54;

module complexObject()
{
	difference()
	{
		union()
		{
			cylinder(r=co_radius, h=co_height);
			rotate([0, 0, r*360/3]) translate([co_radius/1.6, 0, 0])	 cylinder(r=3.82, h=23);

			for(r=[0:5])
			{

			}
		}
		
		union()
		{
			for(r=[0:4])
			{
				rotate([0,0,r*360/1]) translate([co_radius/29.86, 0, 0])	 cylinder(r=7.96, h=co_height*1.1);
				rotate([0, 0, r*360/3]) translate([co_radius/4, 0, 0])	 square(r=4, h=8);

			}
		}
	}
}



complexObject();

