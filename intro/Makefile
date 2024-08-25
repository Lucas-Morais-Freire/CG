COMP = g++
FLAGS = -Wall -Wextra -std=c++20 -O3 # -g

TARGET = main.exe
ARGS =

SRCS = C:\Projetos\glad\src\glad.cpp src\engine.cpp src\shader.cpp src\stb_image.c src\texture.cpp
INCLUDE_PATHS = C:\Projetos\glfw\include C:\Projetos\glad\include C:\Projetos\glm-1.0.1 include
LIBS = opengl32 glfw3 gdi32 # user32
LIB_PATHS = C:\Projetos\glfw\build\src

LIBPATHFLAGS = $(patsubst %,-L%,$(LIB_PATHS))
INCLUDEFLAGS = $(patsubst %,-I%,$(INCLUDE_PATHS))
OBJS = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SRCS))) $(patsubst %.exe,%.o,$(TARGET))
LIBFLAGS = $(patsubst %,-l%,$(LIBS))

# test:
# 	@echo $(OBJS)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(COMP) $(FLAGS) $^ -o $@ $(INCLUDEFLAGS) $(LIBPATHFLAGS) $(LIBFLAGS)

main.o: main.cpp include\engine.hpp include\shader.hpp include\stb_image.h
	$(COMP) $(FLAGS) -c $< -o $@ $(INCLUDEFLAGS) $(LIBPATHFLAGS) $(LIBFLAGS)
src\engine.o: src\engine.cpp include\engine.hpp
	$(COMP) $(FLAGS) -c $< -o $@ $(INCLUDEFLAGS) $(LIBPATHFLAGS) $(LIBFLAGS)
src\shader.o: src\shader.cpp include\shader.hpp
	$(COMP) $(FLAGS) -c $< -o $@ $(INCLUDEFLAGS) $(LIBPATHFLAGS) $(LIBFLAGS)
src\texture.o: src\texture.cpp include\texture.hpp
	$(COMP) $(FLAGS) -c $< -o $@ $(INCLUDEFLAGS) $(LIBPATHFLAGS) $(LIBFLAGS)
src\stb_image.o: src\stb_image.c include\stb_image.h
	$(COMP) $(FLAGS) -c $< -o $@ $(INCLUDEFLAGS) $(LIBPATHFLAGS) $(LIBFLAGS)

%.o: %.cpp
	$(COMP) $(FLAGS) -c $< -o $@ $(INCLUDEFLAGS) $(LIBPATHFLAGS) $(LIBFLAGS)

clean:
	del $(OBJS) $(TARGET)

run: $(TARGET)
	.\$(TARGET) $(ARGS)