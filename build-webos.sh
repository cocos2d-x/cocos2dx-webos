#!/bin/sh
#p=$(dirname $_)
#echo "$p"

#path=$(dirname $0)
#path=${path/\./$(pwd)}
#echo $path
p=.

if [ ! -d "lib" ]; then
mkdir "lib"
fi

if [ ! -d "lib/webos" ]; then
mkdir "lib/webos"
fi

if [ ! -d "lib/webos/Debug" ]; then
mkdir "lib/webos/Debug"
fi

if [ ! -d "lib/webos/Release" ]; then
mkdir "lib/webos/Release"
fi

# copy cocosdenshino depended libs into lib/webos/Debug/
#cp CocosDenshion/third_party/fmod/api/lib/*.so lib/webos/Debug

if [ $# -ne 0 ]; then
	if [ $1 = "clean" ]; then
		cd $p/cocos2dx/proj.webos
		make clean

		cd ../../
		cd $p/CocosDenshion/proj.webos
		make clean

		cd ../..
		cd $p/Box2D/proj.webos
		make clean

		cd ../..
		cd $p/chipmunk/proj.webos
		make clean

		cd ../../
		rm -r lib/webos/Debug/*.so
		rm -r lib/webos/Debug/*.a
	fi
	
else
	cd $p/cocos2dx/proj.webos
	echo "**********************building cocos2dx**************************"
	make
	cp -f libcocos2d.so ../../lib/webos/Debug

#        echo "**********************building cocosdenshion*********************"
	cd ../../
	cd $p/CocosDenshion/proj.webos
	make
	cp -f libcocosdenshion.so ../../lib/webos/Debug

        echo "**********************building Box2D******************************"
	cd ../..
	cd $p/Box2D/proj.webos
	make
	cp -f libbox2d.a ../../lib/webos/Debug

        echo "**********************building chipmunk***************************"
	cd ../..
	cd $p/chipmunk/proj.webos
	make
	cp -f libchipmunk.a ../../lib/webos/Debug

	cd ../../
fi

