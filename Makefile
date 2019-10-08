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
DRIVER ?= not_defined

ifneq ("$(wildcard /opt/vc/include/bcm_host.h)","")
	ifeq ($(shell cat /proc/cpuinfo | grep 'Revision' | awk '{print $$3}' ), c03111)
		PLATFORM = RPI4
		ifeq ($(DRIVER),not_defined)
			DRIVER = gbm
		endif
	else
		PLATFORM = RPI
		ifeq ($(DRIVER),not_defined)
			DRIVER = vc
		endif
	endif
else
	PLATFORM = $(shell uname)
	ifeq ($(DRIVER),not_defined)
		DRIVER = glfw
	endif
endif

$(info ${PLATFORM} platform with $(DRIVER) drivers)

INCLUDES +=	-Isrc/ -Iinclude/
CFLAGS += -Wall -O3 -std=c++11 -fpermissive

ifeq ($(DRIVER),vc)
	CFLAGS += -DGLM_FORCE_CXX14 -DPLATFORM_RPI -Wno-psabi
	INCLUDES += -I/opt/vc/include/ \
				-I/opt/vc/include/interface/vcos/pthreads \
				-I/opt/vc/include/interface/vmcs_host/linux
	LDFLAGS +=  -L/opt/vc/lib/ \
				-lbcm_host \
				-lpthread

	ifeq ($(shell . /etc/os-release && echo $$PRETTY_NAME),Raspbian GNU/Linux 8 (jessie))
	-	LDFLAGS += -lGLESv2 -lEGL
	else
		LDFLAGS += -lbrcmGLESv2 -lbrcmEGL
	endif
else ifeq ($(DRIVER),gbm)
	CFLAGS += -DPLATFORM_RPI4 -Wno-psabi
	INCLUDES += -I/usr/include/libdrm \
				-I/usr/include/GLES2
	LDFLAGS +=  -lGLESv2 -lEGL \
				-ldrm -lgbm \
				-lpthread
	
else ifeq ($(PLATFORM),Linux)
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
