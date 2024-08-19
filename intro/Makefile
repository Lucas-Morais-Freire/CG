COMP = g++
TARGET = intro.exe
SRCS = src\glad.cpp
FLAGS = -Wall -Wextra -O3
LIBS = opengl32 glfw3 gdi32 # user32
INCLUDE_PATHS = C:\Projetos\glfw\include C:\Projetos\github\CG\intro\include
LIB_PATHS = C:\Projetos\glfw\build\src
ARGS =

LIBPATHFLAGS = $(patsubst %,-L%,$(LIB_PATHS))
INCLUDEFLAGS = $(patsubst %,-I%,$(INCLUDE_PATHS))
OBJS = $(patsubst %.exe,%.o,$(TARGET)) $(patsubst %.cpp,%.o,$(SRCS))
LIBFLAGS = $(patsubst %,-l%,$(LIBS))

# test:
# 	@echo $(OBJS)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(COMP) $(FLAGS) $^ -o $@ $(INCLUDEFLAGS) $(LIBPATHFLAGS) $(LIBFLAGS)

%.o: %.cpp
	$(COMP) $(FLAGS) -c $^ -o $@ $(INCLUDEFLAGS) $(LIBPATHFLAGS) $(LIBFLAGS)

clean:
	del $(OBJS) $(TARGET)

run: $(TARGET)
	.\$(TARGET) $(ARGS)