![Fusion](/logo.png)

### Engineering preview!
### No binary builds yet - Early beta!

### Intro
Welcome! This is the code of Fusion Engine, a 2D and 3D engine forked from the Godot Engine. 
The idea is to have an ultra performant game engine, capable of running on a potato. 
Including support for many platforms. 
Fusion Engine is based on Godot 1.0, with new features and fixed bugs.

### Building
Get scons and python, choose a platform from the platform directory and then run 
`scons platform=name target=release tools=no -j#` (Replace "name" with the desired platform)(Replace # in "-j#" with your processors core count).

If you're building on Linux make sure to install libraries for alsa as well as opengl.

If you are on Fedora or a Fedora based distro you can install said required libraries with `sudo dnf install alsa-lib-devel glfw-devel`.
If you are on Debian or Debian based distro you can install said required libraries with `sudo apt install alsa-lib libglfw3-dev`.
Other Linux distros please refer to your package manager for an equivalent.

Every console port uses their specific open source homebrew sdk.

**IF YOU ARE COMPILING FOR WINDOWS, YOU MUST USE OUR CUSTOM TOOLCHAIN FOR IT, FOUND [HERE](https://fusionengine.org/#urn:uuid:e43557be-a278-11ef-b9f3-ca7548087f6a)**

### Making a game
Make a game in the editor.
The editor is unusual if you are familiar with godot, but you'll be able to notice it's very intuitive after learning it. 
Games can range from 3D to 2D to apps. 
The main focus point of the engine has been the 3D part so far.

### Running a game
For all platforms so far, it's as simple as copying the data.pck (or project directory) into the same folder as the fusion engine binary.

### Website
[Fusion Engine](https://fusionengine.org)
https://fusionengine.org

