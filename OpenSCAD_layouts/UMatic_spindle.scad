
// globals
co_radius = 40;
co_height = 50;

module complexObject()
{
	difference()
	{
		union()
		{
			cylinder(r=co_radius, h=co_height, center=true);
			cylinder(r=50, h=5, center=true);

			for(r=[0:5])
			{

			}
		}
		
		union()
		{
			for(r=[0:4])
			{
				rotate([0,0,r*360/3]) translate([co_radius/1.5, 0, 0])	 cylinder(r=8, h=co_height*1.1, center=true);
				rotate([0,0,r*360/1]) translate([co_radius/40, 0, 0])	 cylinder(r=15, h=co_height*1.1, center=true);

			}
		}
	}
}



complexObject();

