# OpenGL-C-Render

A quick simple rendering engine to be designed for later personal project light-weight use. The goal of this render engine is not to be optimised for all usages and operations, or even to be extravagantly optimised in any other way, but rather to be a framework for other projects that will then have a basis for development and no longer need as much work. This is built for the source code to be adapted and changed.

While currently platform-dependent, the aim is to restructure later to become platform-agnostic as best as possible.

Orignially based on the [OpenGL tutorial for beginners](http://www.opengl-tutorial.org/beginners-tutorials) and [OpenGL tutorial for intermediates](http://www.opengl-tutorial.org/intermediate-tutorials/), later repurposed for private use.

[libs](./libs) folder to be replaced for platform-agnosticism. <br>
[engine](./source/engine/) folder contains all of the code that is required for the render engine. <br>
[render_engine.h](./source/engine/render_engine.h) contains the interface for using the render engine. <br>
[ext](./source/ext/) folder contains all of the code that can be used for basic/common goals (i.e. movement) as well as all objects and textures to be used for the render engine. <br>
[core.c](./source/core.c) contains the instantiation and constants for use during runtime. <br>

See render engine documentation [here](./source/engine/doc.md)

## Build and Execution

[core.c](./source/core.c) can be built (with the main func) using
```bash
make build
```
The build time error log file will be built [here](./build_error.log) at ./build_error.log


[core.c](./source/core.c) can be run (with the main func) using
``` bash
make run
```
