
CC := gcc
LIB_PATH := .\libs
C_FLAGS := -Wall -g -DGLEW_STATIC
LNK_LIBS := -lglfw3 -lgdi32 -lopengl32 -lm -lGLU32 -lGLEW32
INC_LIBS := $(LIB_PATH)\ $(LIB_PATH)\glfw3dll.lib $(LIB_PATH)\OpenGL32.lib $(LIB_PATH)\glew32.lib

MAIN_FILE := core.c
COMP_FILES := ./shaders/shader.c 
COMP_FILES += ./source/vectors/vector3.c 
COMP_FILES += ./source/vectors/vector4.c 
COMP_FILES += ./source/vectors/matrix_4sq.c 
COMP_FILES += ./source/render_engine.c 
COMP_FILES += $(MAIN_FILE)

C_FLAGS += -D_WIN64

.Phony: build
build:
	$(CC) $(COMP_FILES) $(C_FLAGS) $(LNK_LIBS) -I $(INC_LIBS) -o render.exe 2> build_error.log

.Phony: run
run:
	./render.exe
