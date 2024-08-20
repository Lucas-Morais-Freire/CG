COMP = g++
FLAGS = -Wall -Wextra -O3

TARGET = main.exe
ARGS =

SRCS = C:\Projetos\glad\src\glad.cpp src\auxfuncs.cpp
INCLUDE_PATHS = C:\Projetos\glfw\include C:\Projetos\glad\include include
LIBS = opengl32 glfw3 gdi32 # user32
LIB_PATHS = C:\Projetos\glfw\build\src

LIBPATHFLAGS = $(patsubst %,-L%,$(LIB_PATHS))
INCLUDEFLAGS = $(patsubst %,-I%,$(INCLUDE_PATHS))
OBJS = $(patsubst %.exe,%.o,$(TARGET)) $(patsubst %.cpp,%.o,$(SRCS))
LIBFLAGS = $(patsubst %,-l%,$(LIBS))

# test:
# 	@echo $(OBJS)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(COMP) $(FLAGS) $^ -o $@ $(INCLUDEFLAGS) $(LIBPATHFLAGS) $(LIBFLAGS)

main.o: main.cpp include\auxfuncs.hpp
	$(COMP) $(FLAGS) -c $< -o $@ $(INCLUDEFLAGS) $(LIBPATHFLAGS) $(LIBFLAGS)

src\auxfuncs.o: src\auxfuncs.cpp include\auxfuncs.hpp
	$(COMP) $(FLAGS) -c $< -o $@ $(INCLUDEFLAGS) $(LIBPATHFLAGS) $(LIBFLAGS)

%.o: %.cpp
	$(COMP) $(FLAGS) -c $< -o $@ $(INCLUDEFLAGS) $(LIBPATHFLAGS) $(LIBFLAGS)

clean:
	del $(OBJS) $(TARGET)

run: $(TARGET)
	.\$(TARGET) $(ARGS)