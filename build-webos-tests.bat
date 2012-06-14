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
xcopy /E Res ..\testsWebos\Res\
copy Res\Images\Icon.png ..\testsWebos\
cd ..\testsWebos\

@echo {"title": "cocos2dx-test","type": "pdk","main": "cocos2dx-test","icon": "Icon.png","id": "com.palm.app.cocos2dx-test",	"version": "1.3.0",	"vendor": "My","requiredMemory": 100} >appinfo.json
@echo filemode.755=cocos2dx-test, libcocos2d.so, libcocosdenshion.so, libbox2d.a, libchipmunk.a>package.properties
cd ..\
del /Q Box2D\proj.webos\libbox2d.a
del /Q chipmunk\proj.webos\libchipmunk.a
del /Q cocos2dx\proj.webos\libcocos2d.so
del /Q CocosDenshion\proj.webos\libcocosdenshion.so
del /Q tests\test.webos\cocos2dx-test
palm-package testsWebos

pause


