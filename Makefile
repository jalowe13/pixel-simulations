# Makefile for The One
# Jacob Lowe

CXX=g++



# VCPKG Source using the current user profile
MSYS2_ROOT := C:/vcpkg
MSYS2_TARGET_TRIPLET := mingw64

#Icon
# Application Icon
ICON = icon.ico
# Resource Script
RC = icon.rc
# Icon Object
RES = icon.o

# Swap between Windows and Unix installs
ifeq ($(OS),Windows_NT)
    RM = del /Q
	CXXFLAGS = -I$(MSYS2_ROOT)/$(MSYS2_TARGET_TRIPLET)/include
	LDFLAGS := -L$(MSYS2_ROOT)/$(MSYS2_TARGET_TRIPLET)/lib
	LIB_LIST = -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -ljsoncpp -static-libgcc -static-libstdc++
else
    RM = rm -f
	LDFLAGS= -Lusr/include
	LIB_LIST = -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -ljsoncpp
endif



all: C23Engine
C23Engine: main.o Application.o icon.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) main.o Application.o icon.o -o pixels  $(LIB_LIST)
main.o: main.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c main.cpp $(LIB_LIST)
Application.o: Application.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c Application.cpp $(LIB_LIST)
icon.o: icon.rc
	windres -O coff -i icon.rc -o icon.o

# Swap between Windows and Unix installs
ifeq ($(OS),Windows_NT)
    RM = del /Q
else
    RM = rm -f
endif

# Clean all object files
cleanall:
	$(RM) *.o

# Clean specific object file, used more if a header file is edited and not detected as a change
clean:
	$(RM) $(addsuffix .o,$(filter-out $@,$(MAKECMDGOALS)))