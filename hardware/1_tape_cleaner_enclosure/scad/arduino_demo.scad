include<arduino.scad>

translate([-50, -40, 0]) Arduino(0, 0, 0);

translate([50, -40, 0]) Arduino(0, 1, 0);

translate([-50, 40, 0]) Arduino(1, 0, 0);

translate([50, 40, 0]) Arduino(1, 1, 0);

translate([150, -40, 0]) Arduino(0, 0, 1);

translate([250, -40, 0]) Arduino(0, 1, 1);

translate([150, 40, 0]) Arduino(1, 0, 1);

translate([250, 40, 0]) Arduino(1, 1, 1);