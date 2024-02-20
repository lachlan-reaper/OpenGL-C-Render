
CC := gcc
C_FLAGS := -Wall -g -DGLEW_STATIC -D_WIN64
LNK_LIBS := -lglfw3 -lgdi32 -lopengl32 -lm -lGLU32 -lGLEW32
INC_LIBS := .\includes\ .\includes\glfw3dll.lib .\includes\OpenGL32.lib .\includes\glew32.lib

MAIN_FILE := core.c
COMP_FILES := ./shaders/shader.c 
COMP_FILES += $(MAIN_FILE)

.Phony: build
build:
	$(CC) $(COMP_FILES) $(C_FLAGS) $(LNK_LIBS) -I $(INC_LIBS) -o render.exe

.Phony: run
run:
	./render.exe
