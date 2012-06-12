@Echo Off

If Not Exist "%testsWebos%" md testsWebos

del /Q testsWebos\*.*

cd Box2D\proj.webos
cs-make
copy libbox2d.a ..\..\testsWebos\

cd ..\..\chipmunk\proj.webos
cs-make
copy libchipmunk.a ..\..\testsWebos\

cd ..\..\cocos2dx\proj.webos
cs-make
copy libcocos2d.so ..\..\testsWebos\

cd ..\..\CocosDenshion\proj.webos
cs-make
copy libcocosdenshion.so ..\..\testsWebos\

cd ..\..\tests\test.webos
cs-make
copy cocos2dx-test ..\..\testsWebos\

cd ..\
xcopy Res ..\testsWebos\Res\
copy Res\Images\Icon.png ..\testsWebos\
cd ..\testsWebos\

@echo {"title": "cocos2dx-test","type": "pdk","main": "cocos2dx-test","icon": "Icon.png","id": "com.palm.app.cocos2dx-test",	"version": "1.3.0",	"vendor": "My","requiredMemory": 15} >appinfo.json
@echo filemode.755=cocos2dx-test>package.properties
cd ..\
palm-package testsWebos

pause


