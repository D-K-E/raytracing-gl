# raytracing-gl
OpenGL compute shader port of 'Raytracing in One Weekend' by P. Shirley

This repository follows closely the examples introduced by the 
phenomenal "Raytracing in One Weekend" by P. Shirley.

I did unfortunately not document the code much, since Shirley gives a through
explanation of how everything works. Assuming you have basic knowledge of
opengl and glsl, it should be fairly easy to understand what's going on in the
shader code.

Due to the imperative nature of glsl, I have, from time to time, diverged from
the implementation of Shirley.

### Requirements

These requirements basically mimick my development environment. Some of them can
probably be lowered.

- Cmake >= 3.0.2
- OpenGL >= 4.3
- GLFW 
- Glad function pointers for OpenGL 4.3
- C++ 17.

## Compiling the code

- Create "build" directory in main folder containing this README.md
- `cd build`
- `cmake ..`
- `make install`

There will be two copies of executables one in build folder another in `bin`
folder.
Use the ones in `bin` folder since reference to any media is hardcoded inside
the code with respect to relative to `bin`.

## Remarks

You'll notice that most of the implementation files, `.cpp`s, are same in nature
maybe diverging 1 or 2 lines somewhere.


