CC := gcc
LIB_PATH := .\libs
C_FLAGS := -Wall -g 
LNK_LIBS := -lglfw3 -lgdi32 -lopengl32 -lm -lGLU32 -lGLEW32
INC_LIBS := $(LIB_PATH)\ $(LIB_PATH)\glfw3dll.lib $(LIB_PATH)\OpenGL32.lib $(LIB_PATH)\glew32.lib

SRC_PATH := ./source
MAIN_FILE := $(SRC_PATH)/core.c
ENG_PATH := $(SRC_PATH)/engine
EXT_PATH := $(SRC_PATH)/ext
COMP_FILES := $(ENG_PATH)/shaders/shader.c 
COMP_FILES += $(ENG_PATH)/textures/texture_loader.c
COMP_FILES += $(ENG_PATH)/vectors/vector2.c 
COMP_FILES += $(ENG_PATH)/vectors/vector3.c 
COMP_FILES += $(ENG_PATH)/vectors/vector4.c 
COMP_FILES += $(ENG_PATH)/vectors/matrix_4x4.c 
# COMP_FILES += $(ENG_PATH)/vectors/dyn_array.c 	TBU: WHEN CODE IS ADDED
COMP_FILES += $(ENG_PATH)/objects/camera.c 
COMP_FILES += $(ENG_PATH)/objects/object_loader.c
COMP_FILES += $(ENG_PATH)/render_engine.c 
COMP_FILES += $(ENG_PATH)/default/basic_3d_functions.c

COMP_FILES += $(EXT_PATH)/movement.c
COMP_FILES += $(EXT_PATH)/default/basic_movement.c

COMP_FILES += $(MAIN_FILE)

C_FLAGS += -DGLEW_STATIC
C_FLAGS += -D_WIN64

.PHONY: build run
.SILENT: build run
build:
	echo Compiling program
	$(CC) $(C_FLAGS) $(COMP_FILES) $(LNK_LIBS) -I $(INC_LIBS) -o render.exe 2> build_error.log
	echo Finished compiling
	echo.

run:
	echo Executing program
	./render.exe
	echo Closed program
	echo.
