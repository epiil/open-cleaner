
$fn=100;

module roller() {

	difference() 
	{
		union() 
		color("black")
		{
		resize(newsize=[45,45,6.3]) sphere(r=20); 
		cylinder(h=21.08, d=21.20, $fs=.01); //base
		cylinder(r1=21.20, r2=15, h=2.36, $fs=.01);
		translate([0,0,19.9])
		resize(newsize=[45,45,6.3]) sphere(r=20);
		}
		union()
		color("black")
		{	
		cylinder(h=80, d=11, center = true, $fs=.01);
		cylinder(h=5.1, d=16, $fs=.01);
		translate([0,0,-2])
		cylinder(h=4, d=16, center=true, $fs=.01);
		translate([0,0,19.2])
		cylinder(h=5.2, d=16, $fs=.01);
		}
	}
}

roller();