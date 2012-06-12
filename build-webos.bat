@Echo Off

If Not Exist "%lib%\*.*" md lib
If Not Exist "%lib%\%webos%\*.*" md lib\webos
If Not Exist "%lib%\%webos%\%Debug%\*.*" md lib\webos\Debug
If Not Exist "%lib%\%webos%\%Release%\*.*" md lib\webos\Release

del /Q lib\webos\Debug\*.so
del /Q lib\webos\Debug\*.a

cd Box2D\proj.webos
cs-make
copy libbox2d.a ..\..\lib\webos\Debug\

cd ..\..\chipmunk\proj.webos
cs-make
copy libchipmunk.a ..\..\lib\webos\Debug\

cd ..\..\cocos2dx\proj.webos
cs-make
copy libcocos2d.so ..\..\lib\webos\Debug\

cd ..\..\CocosDenshion\proj.webos
cs-make
copy libcocosdenshion.so ..\..\lib\webos\Debug\
pause


