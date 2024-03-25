# Objects

This document is kept up to date as best as possible but may lag behind current code.

## Camera Interface
The header file is located [here](./camera.h).<br>
The source code file is located [here](./camera.c).
<br><br>

Please note that the general program flow of using this interface is:
1. Initialisation (Externally creating memory)
2. Setting camera defaults
3. During run-time
	1. Update Vectors
	2. Calculate new V, P and VP matrices
4. Freeing the camera object
<br><br>

|Functions|Description|Return Value|
|---------|-----------|------------|
| void set_camera(<br>&emsp;Camera* const camera, <br>&emsp;const VECTOR_FLT horizontal_angle, <br>&emsp;const VECTOR_FLT vertical_angle, <br>&emsp;const VECTOR_FLT FOV<br>) | This will set a `camera` object to the given parameters and its internal defaults | Returns void |
| static inline void set_camera_position(<br>&emsp;Camera* const camera, <br>&emsp;const VECTOR_FLT x, <br>&emsp;const VECTOR_FLT y, <br>&emsp;const VECTOR_FLT z<br>) | Sets camera position | Returns void |
| static inline void update_vectors(<br>&emsp;Camera* const camera<br>) | Recalculates the right, forward and up directions and stores the result internally | Returns void |
| static inline void update_angles(<br>&emsp;Camera* const camera, <br>&emsp;const VECTOR_FLT delta_horz, <br>&emsp;const VECTOR_FLT delta_vert<br>) | Modifies the `horizontal_angle` and `vertical_angle` accordingly by the delta provided; no value check | Returns void |
| static inline void update_position(<br>&emsp;Camera* const camera, <br>&emsp;const VECTOR_FLT delta_forward, <br>&emsp;const VECTOR_FLT delta_right, <br>&emsp;const VECTOR_FLT delta_up<br>) | Modifies the current position of the camera according to the provided deltas relative to the current orientation of the camera | Returns void |
| static inline void update_FOV(<br>&emsp;Camera* const camera, <br>&emsp;const VECTOR_FLT delta_FOV<br>) | Updates the current FOV value by the provided delta | Returns void |
| static inline void camera_look_at(<br>&emsp;const Camera* const camera, <br>&emsp;matrix_4x4* view<br>) | Sets the `view` matrix to be the View matrix transform based on the current orientation and position of the camera | Returns void |
| static inline void camera_perspective(<br>&emsp;const Camera* const camera, <br>&emsp;matrix_4x4* perspective, <br>&emsp;const int window_width, <br>&emsp;const int window_height<br>) | Sets the `perspective` transform matrix based on the current FOV value of the camera and the current screen size/aspect ratios | Returns void | 
| static inline void calc_camera_vp(<br>&emsp;Camera* const camera, <br>&emsp;const int window_width, <br>&emsp;const int window_height<br>) | Recalculates and stores internally the view, perspective and VP matrices | Returns void |

## Model Interface
The header file is located [here](./model.h).<br>
The source code file is located [here](./model.c).
<br><br>

Please note that the general program flow of using this interface is:
1. Initialisation
2. Load object to model
3. Load texture to model
4. Add as many model instances as needed
5. During run-time:
	1. Nothing changes; values are accessed
	2. Clean up object
4. Freeing the model object
<br><br>

|Functions|Description|Return Value|
|---------|-----------|------------|
| Model* newModel( void ) | Allocates memory and intialises a Model object | Returns a pointer to the newly allocated object |
| void initialiseModel(<br>&emsp;Model* const model<br>) | Initilises and sets all internal memory and defaults | Returns void |
| void loadObjectToModel(<br>&emsp;Model* const model, <br>&emsp;const char* path<br>) | Loads a .DSS object as specified at `path` into the model object for later use | Returns void |
| void loadTextureToModel(<br>&emsp;Model* const model, <br>&emsp;const char* path<br>) | Loads a .uv texture as specified at `path` into the model object for later use | Returns void |
| MODEL_INST_ID_TYPE addModelInstance(<br>&emsp;Model* const model, <br>&emsp;const vector3 coors, <br>&emsp;const vector3 scale, <br>&emsp;const vector3 rotation<br>) | Allocates, creates and adds a new instance of the model with the values provided | Returns the instance ID of the newly created instance |
| void clean_model(<br>&emsp;Model* model<br>) | Frees and cleans all associated internal memory within the model | Returns void |

