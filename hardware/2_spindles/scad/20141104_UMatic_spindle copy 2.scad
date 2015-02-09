
// globals
co_radius = 60;
co_height = 2;

module complexObject()
{
	difference()
	{
		union()
		{
			cylinder(r=co_radius, h=co_height);
			cylinder(r=13.5, h=20);
			{

			}
		}
		
		difference()
		{
			cylinder(r=4, h=20);
			{

			}
		}
	}
}



complexObject();

