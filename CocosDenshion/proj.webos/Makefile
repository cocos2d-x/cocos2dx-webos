#MAKE FILE FOR WEBOS
CC      = arm-none-linux-gnueabi-gcc
CXX     = arm-none-linux-gnueabi-g++
TARGET	= libcocosdenshion.so
CCFLAGS = -Wall -g -O2 -fPIC -Wl,--allow-shlib-undefined
CXXFLAGS = -Wall -g -O2 -fPIC -Wl,--allow-shlib-undefined
VISIBILITY = 

INCLUDES = -I.. \
					-I../include \
					-I"C:\Program Files (x86)\HP webOS\PDK\include" \
					-I"C:\Program Files (x86)\HP webOS\PDK\include\SDL" \
					-L"C:\Program Files (x86)\HP webOS\PDK\device\lib" 

DEFINES = -DWEBOS

OBJECTS =   ../WebOS/SDLAudioPlayer.o \
            ../WebOS/SimpleAudioEngine.o \

SHAREDLIBS = -lSDL -lSDL_Mixer -lGLES_CM -lpdl -lpthread-2.5

####### Build rules  $(STATICLIBS)
$(TARGET): $(OBJECTS) 
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEFINES) $(OBJECTS) -shared -o $(TARGET) $(SHAREDLIBS)
	
####### Compile
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEFINES) $(VISIBILITY) -c $< -o $@

%.o: %.c
	$(CC) $(CCFLAGS) $(INCLUDES) $(DEFINES) $(VISIBILITY) -c $< -o $@
		
clean: 
	rm -f $(OBJECTS) $(TARGET) core

