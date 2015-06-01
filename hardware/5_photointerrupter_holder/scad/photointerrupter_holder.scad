$fn=100;

module photointerrupterroller() {

	difference() 
	{
		union() 
		color("black")
		{
		cube([35.44,24.50,4.58]);
		}
		union()
		color("black")
		{	
		translate([7,0,0])
		cube([21,15.15,2.7]);
		translate([7,-1,0])
		cube([21,15.15,2.7]);
		translate([7,0,-1])
		cube([21,15.15,1.60]);
		translate([7,10,-1])
		cube([21,6,6]);
		translate([4,19,0])
		cube([28,2.5,5]);
		translate([3,17,3])
		cube([30,9,2]);
		translate([4,19,-1])
		cube([28,2.5,5]);
		}
	}
}

photointerrupterroller();
