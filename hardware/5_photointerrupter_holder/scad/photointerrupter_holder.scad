$fn=100;

module photointerrupterroller() {

	difference() 
	{
		union() 
		color("black")
		{
		cube([35.44,26,4.58]);
		}
		union()
		color("black")
		{	
		translate([7,0,0])
		cube([21,15.15,3]);
		translate([7,-1,0])
		cube([21,15.15,3]);
		translate([7,0,-1])
		cube([21,15.5,1.60]);
		translate([7,10,-1])
		cube([21,6,6]);
		translate([17.2,21,2.95])
		cylinder(h=5,d=7.14);
		translate([17.2,21,0])
		cylinder(h=5,d=2.95);
		translate([17.2,21,-1])
		cylinder(h=5,d=2.95);
		//cube([28,2.5,5]);
		//translate([3,17,3])
		//cube([30,9,2]);
		//translate([4,19,-1])
		//cube([28,2.5,5]);
		}
	}
}

photointerrupterroller();
