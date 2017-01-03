@echo off
rem See: http://en.wikibooks.org/wiki/OpenSCAD_User_Manual/Using_OpenSCAD_in_a_command_line_environment

echo Making simple box for stand alone silica gem modules
"C:\Program Files (x86)\OpenSCAD\openscad.exe" -o .\SilicaGelCase.stl -m --render .\SilicaGelCase.scad
"C:\Program Files (x86)\OpenSCAD\openscad.exe" -o .\SilicaGelCaseLid.stl -m --render .\SilicaGelCaseLid.scad