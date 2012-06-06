#WEBOS HELLOWORLD MAKE FILE
CC      = arm-none-linux-gnueabi-gcc
CXX     = arm-none-linux-gnueabi-g++
TARGET	= HelloWorld
CCFLAGS = -Wall -g -O2 -fPIC -Wl,--allow-shlib-undefined
CXXFLAGS = -Wall -g -O2 -fPIC -Wl,--allow-shlib-undefined
VISIBILITY = 

INCLUDES =  -I../../cocos2dx/include \
            -I../../cocos2dx \
            -I../../cocos2dx/platform \
            -I../../CocosDenshion/include \
            -I../Classes/ \
	    -I"C:\Program Files (x86)\HP webOS\PDK\include" \
	    -I"C:\Program Files (x86)\HP webOS\PDK\include\SDL" \
	    -L"C:\Program Files (x86)\HP webOS\PDK\device\lib"

DEFINES = -DWEBOS

OBJECTS = ./main.o \
        ../Classes/AppDelegate.o \
        ../Classes/HelloWorldScene.o 

STATICLIBS =  
#SHAREDLIBS = -L../../lib/linux -lcocos2d -lrt -ljpeg -lSDL -lGLES_CM -lpdl -lpng12 -Wl,-rpath,../../lib/linux/Debug/
#SHAREDLIBS = -L../../lib/linux -lcocos2d
SHAREDLIBS = -L../../cocos2dx/proj.webos/ -lcocos2d

#$(shell ../../build-linux.sh $<)
####### Build rules
$(TARGET): $(OBJECTS) 
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEFINES) $(OBJECTS) -o $(TARGET) $(SHAREDLIBS) $(STATICLIBS)
	
####### Compile
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEFINES) $(VISIBILITY) -c $< -o $@

%.o: %.c
	$(CC) $(CCFLAGS) $(INCLUDES) $(DEFINES) $(VISIBILITY) -c $< -o $@
		
clean: 
	rm -f $(OBJECTS) $(TARGET) core
