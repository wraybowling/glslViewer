EXE = ./bin/glslViewer

CXX = g++
HEADERS := $(wildcard include/*/*.h) $(wildcard src/*.h) $(wildcard src/*/*.h) \
	$(wildcard include/oscpack/osc/*.h)   $(wildcard include/oscpack/ip/posix/*.h)
SOURCES := $(wildcard include/*/*.cc) $(wildcard include/*/*.cpp) $(wildcard src/*.cpp) $(wildcard src/*/*.cpp) \
	$(wildcard include/oscpack/osc/*.cpp) $(wildcard include/oscpack/ip/posix/*.cpp)
#HEADERS := $(shell find include/ src/ -type f -name '*.h')
#SOURCES := $(shell find include/* include/ip/posix src src/* -maxdepth 1 -type f -name '*.cc' -o -name '*.cpp')
OBJECTS := $(SOURCES:.cpp=.o)

PLATFORM = $(shell uname)
ifneq ("$(wildcard /etc/os-release)","")
PLATFORM = $(shell . /etc/os-release && echo $$NAME)
endif

#override platform selection on RPi:
ifneq ("$(wildcard /opt/vc/include/bcm_host.h)","")
    PLATFORM = $(shell . /etc/os-release && echo $$PRETTY_NAME)
endif

#$(info Platform ${PLATFORM})

INCLUDES +=	-Isrc/ -Iinclude/
CFLAGS += -Wall -O3 -std=c++11 -fpermissive

ifeq ($(PLATFORM),Raspbian GNU/Linux 8 (jessie))
CFLAGS += -DGLM_FORCE_CXX98 -DPLATFORM_RPI
INCLUDES += -I$(SDKSTAGE)/opt/vc/include/ \
			-I$(SDKSTAGE)/opt/vc/include/interface/vcos/pthreads \
			-I$(SDKSTAGE)/opt/vc/include/interface/vmcs_host/linux
LDFLAGS += -L$(SDKSTAGE)/opt/vc/lib/ \
			-lGLESv2 -lEGL \
			-lbcm_host \
			-lpthread

else ifeq ($(PLATFORM),Raspbian GNU/Linux 9 (stretch))
	CFLAGS += -DGLM_FORCE_CXX98 -DPLATFORM_RPI
	INCLUDES += -I$(SDKSTAGE)/opt/vc/include/ \
				-I$(SDKSTAGE)/opt/vc/include/interface/vcos/pthreads \
				-I$(SDKSTAGE)/opt/vc/include/interface/vmcs_host/linux
	LDFLAGS += -L$(SDKSTAGE)/opt/vc/lib/ \
			   	-lbrcmGLESv2 -lbrcmEGL \
			   	-lbcm_host \
			    -lpthread
$(info Platform ${PLATFORM})

else ifeq ($(shell uname),Linux)
CFLAGS += -DPLATFORM_LINUX $(shell pkg-config --cflags glfw3 glu gl)
LDFLAGS += $(shell pkg-config --libs glfw3 glu gl x11 xrandr xi xxf86vm xcursor xinerama xrender xext xdamage) -lpthread -ldl

else ifeq ($(PLATFORM),Darwin)
CXX = /usr/bin/clang++
ARCH = -arch x86_64
CFLAGS += $(ARCH) -DPLATFORM_OSX -stdlib=libc++ $(shell pkg-config --cflags glfw3)
INCLUDES += -I/System/Library/Frameworks/GLUI.framework
LDFLAGS += $(ARCH) -framework OpenGL -framework Cocoa -framework CoreVideo -framework IOKit $(shell pkg-config --libs glfw3)

endif

all: $(EXE)

%.o: %.cpp
	@echo $@
	$(CXX) $(CFLAGS) $(INCLUDES) -g -c $< -o $@ -Wno-deprecated-declarations

$(EXE): $(OBJECTS) $(HEADERS)
	$(CXX) $(CFLAGS) $(OBJECTS) $(LDFLAGS) -rdynamic -o $@

clean:
	@rm -rvf $(EXE) src/*.o src/*/*.o include/*/*.o *.dSYM
	
install:
	@cp $(EXE) /usr/local/bin
	@cp bin/glslLoader /usr/local/bin

install_python:
	@python setup.py install
	@python3 setup.py install

clean_python:
	@rm -rvf build
	@rm -rvf dist]
	@rm -rvf python/glslviewer.egg-info

uninstall:
	@rm /usr/local/$(EXE)
	@rm /usr/local/bin/glslLoader
