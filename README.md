##open-cleaner: An (in-progress) open-source tape cleaning solution for analog video / magnetic media

##Repository Contents:

- /hardware - contains .scad and .stl files for 3D CAD / parts printing. All 3D modeling is done with OpenSCAD (http://www.openscad.org/) and Tinkercad (https://tinkercad.com/). All circuit schematics are done with Fritzing (http://fritzing.org/home/)

- /software - contains .ino Arduino "sketches" and Arduino libraries (http://www.arduino.cc/)

- /misc - contains cleaning parameters, parts lists, photos, schematics, etc. 
- 
├── History.md
├── Introduction.md
├── LICENSE.md
├── README.md
├── hardware
│   ├── 1_tape_cleaner_enclosure
│   │   ├── dxf
│   │   │   ├── BackPanel.dxf
│   │   │   ├── FrontPanel.dxf
│   │   │   └── TopPanel.dxf
│   │   ├── scad
│   │   │   ├── Arduino.scad
│   │   │   ├── BackPanel2D.scad
│   │   │   ├── FrontPanel2D.scad
│   │   │   ├── TapeCleaner.scad
│   │   │   └── TopPanel2D.scad
│   │   └── stl
│   │       └── TapeCleaner.stl
│   ├── 2_spindles
│   │   ├── scad
│   │   │   ├── 20141022_QuarterInchHalfInchSpindle.scad
│   │   │   ├── 20141104_UMatic_spindle copy 2.scad
│   │   │   ├── 20141104_UMatic_spindle copy.scad
│   │   │   ├── 20141104_UMatic_spindle.scad
│   │   │   ├── 20150209_Umatic.scad
│   │   │   └── MotorSpindle.scad
│   │   └── stl
│   │       ├── 20141104_Halfinch_spindle.stl
│   │       ├── 20141104_UMatic_spindleNov414.stl
│   │       ├── 20141104_UMatic_spindleOct2214.stl
│   │       ├── 20150115_UMatic_spindletakeup copy.stl
│   │       ├── 20150115_UMatic_spindletakeup.stl
│   │       └── MotorSpindle.stl
│   └── 3_schematics
│       ├── fzz
│       │   ├── 20141115_tape_cleaner.fzz
│       │   └── 20141116_tape_cleaner.fzz
│       ├── pdf
│       │   ├── 20141115_tape_cleaner_schem.pdf
│       │   ├── 20141116_tape_cleaner_schem.pdf
│       │   └── Arduino_Uno_Rev3-schematic.pdf
│       └── png
│           ├── 20141115_tape_cleaner_schem.png
│           └── 20141116_tape_cleaner_schem.png
├── index.html
├── misc
│   ├── 1_parts_lists
│   │   └── PartsList.md
│   ├── 2_part_dimensions
│   │   ├── enclosure
│   │   │   ├── enclosure.md
│   │   │   └── enclosure_diagram.png
│   │   ├── format_hubs
│   │   │   ├── HalfInch_reel_dimensions.txt
│   │   │   └── Umatic_hub_dimensions.txt
│   │   ├── pellon_cleaning_path
│   │   │   └── pellon_cleaning_path_guides.txt
│   │   └── tape_cleaner_spindles
│   │       ├── Halfinch_spindle_dimensions.txt
│   │       └── UMatic_spindle_dimensions.txt
│   ├── 3_parameters
│   │   ├── arduino_MotorShield_Rev3
│   │   │   ├── Shield_Motor-REV3c.brd
│   │   │   └── Shield_Motor-REV3c.sch
│   │   └── cleanerparameters.md
│   └── 4_in_progress_photos
│       ├── 20150115_UMatic_spindletakeup.JPG
│       ├── CleanerWithPartsDiagonalView.png
│       ├── CleanerWithPartsFront.png
│       ├── proto_PellonGuideLayout.jpg
│       ├── proto_PellonGuideTop.jpg
│       └── proto_spindlemod.JPG
└── software
    ├── ino
    │   ├── legacy_sketches
    │   │   ├── sketch_oct14.ino
    │   │   └── sketch_oct28_dcmotor_w_pot.ino
    │   ├── sketch_nov13a_initial_DC_motor_test
    │   │   └── sketch_nov13a_initial_DC_motor_test.ino
    │   ├── sketch_nov15_AB_motor_test
    │   │   └── sketch_nov15_AB_motor_test.ino
    │   ├── sketch_nov15_bidirectional_forward_test
    │   │   └── sketch_nov15_bidirectional_forward_test.ino
    │   ├── sketch_nov15_bidirectional_reverse_test
    │   │   └── sketch_nov15_bidirectional_reverse_test.ino
    │   └── sketch_nov19a_photo_interrupter_test
    │       └── sketch_nov19a_photo_interrupter_test.ino
    └── libraries
        ├── AFMotor
        │   ├── AFMotor.cpp
        │   ├── AFMotor.h
        │   ├── README.txt
        │   ├── examples
        │   │   ├── AFMotor_ConstantSpeed
        │   │   │   └── AFMotor_ConstantSpeed.pde
        │   │   ├── AFMotor_MultiStepper
        │   │   │   └── AFMotor_MultiStepper.pde
        │   │   ├── MotorParty
        │   │   │   └── MotorParty.pde
        │   │   ├── MotorTest
        │   │   │   └── MotorTest.pde
        │   │   └── StepperTest
        │   │       └── StepperTest.pde
        │   └── keywords.txt
        └── readme.txt


