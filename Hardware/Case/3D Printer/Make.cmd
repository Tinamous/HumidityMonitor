@echo off
rem See: http://en.wikibooks.org/wiki/OpenSCAD_User_Manual/Using_OpenSCAD_in_a_command_line_environment

echo Making fan outer shell
"C:\Program Files (x86)\OpenSCAD\openscad.exe" -o .\HumidityCase.stl -m --render .\HumidityCase.scad

echo Making lid
"C:\Program Files (x86)\OpenSCAD\openscad.exe" -o .\HumidityCaseLid.stl -m --render .\HumidityCaseLid.scad

echo Making pipe inlet
"C:\Program Files (x86)\OpenSCAD\openscad.exe" -o .\HumidityCasePipeIn.stl -m --render .\HumidityCasePipeIn.scad

echo Making rod support
"C:\Program Files (x86)\OpenSCAD\openscad.exe" -o .\HumidityRodSupport.stl -m --render .\HumidityRodSupport.scad

echo Making simple box for stand alone silica gem modules
"C:\Program Files (x86)\OpenSCAD\openscad.exe" -o .\SilicaGelCase.stl -m --render .\SilicaGelCase.scad

echo Making padding for spools
"C:\Program Files (x86)\OpenSCAD\openscad.exe" -o .\HumidityRodPadding.stl -m --render .\HumidityRodPadding.scad