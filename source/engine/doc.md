# Render Engine documentation

This document is kept up to date as best as possible but may lag behind current code.

## Sub-docs
- [Objects](./objects/doc.md)
- [Vectors and Matrices](./vectors/doc.md)

## Render Engine Interface
The header file is located [here](./render_engine.h).<br>
The source code file is located [here](./render_engine.c).<br><br>

Please note that the general program flow of using this interface is:
1. Initialisation
2. Setting function pointers
3. Loading Models and Textures
4. Creating and setting Model Instances
5. Running the engine
	1. Priming the engine
	2. Completing per-cycle calculations
	3. Drawing model instances
	4. Cleanup
6. Freeing the Render Engine object
<br><br>

|Flags|Descriptions|
|-----|------------|
|`MAX_INSTANCES_BATCH`|is used to defined the max number of instance drawn in each batch for each model. This is generally limited by the shader and the max size limit there.|

<br>

|Functions|Description|Return Value|
|---------|-----------|------------|
| render_engine_struct* initialiseRenderEngine(<br>&emsp;const int window_width, <br>&emsp;const int window_height<br>) | This will initialise a `render_engine_struct` object | Returns a pointer to the intialised `render_engine_struct` object |
| int primeRenderEngine(<br>&emsp;render_engine_struct* const re_struct<br>) | Calculates, stores and collects the information needed for runtime/drawing that does not change during execution|Returns a `0` on success and a non-`0` on failure |
| int drawRenderEngine(<br>&emsp;render_engine_struct* const re_struct<br>) | Will call the needed functions to update all needed information and call the draw function that has been set | Returns a `0` on succesful completion of the program, returns a non-`0` if any error was encountered during run-time |
| int run(<br>&emsp;render_engine_struct* const re_struct<br>) | Will begin the prime the render engine, compute the execution and draw cycle, and complete cleanup operations following the ending of the program | Returns a `0` on succesful completion of the program, returns a non-`0` if any error was encountered during run-time |
| MODEL_ID_TYPE addModel(<br>&emsp;render_engine_struct* const re_struct, <br>&emsp;const char* obj_path, <br>&emsp;const char* texture_path<br>) | Adds a new model with the shape described in .DSS format at `obj_path` and the UV texture at `texture_path` | Returns the model ID number that corresponds to the newly created model | 
|MODEL_INST_ID_TYPE add_instance_of_model(<br>&emsp;render_engine_struct* const re_struct, <br>&emsp;const MODEL_ID_TYPE model_id, <br>&emsp;const vector3 coords, <br>&emsp;const vector3 scale, <br>&emsp;const vector3 rotation<br>) | Creates a new instance of the model at the position, `coords`, scaled by a factor of `scale` and rotated by `rotation` (in radians) | Returns the instance ID of the newly created instance |
| int cleanupRenderEngine(<br>&emsp;render_engine_struct* const re_struct<br>) | Frees all memory associated with the `render_engine_struct` object | Returns `0` on success, a non-`0` on failure or error. |

