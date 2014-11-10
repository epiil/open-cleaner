cylinder(h = 5, r = 60);
union() {
	cylinder(h = 40, r = 5);
	cube(h = 50, r1 = 60, r2 = 10);
}
	

union() {
  cylinder(h = 5, r = 60);
  cylinder(h = 60, r = 5);
translate([0, 0, 120 * sin(30)])
	sphere(r = 5);
}
