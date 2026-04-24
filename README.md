![Fusion](/logo.png)

### Engineering preview!
### No binary builds yet - Early beta!

### Intro
Welcome! This is the code of Fusion Engine, a 2D and 3D engine forked from Godot Engine 1.0, with new features and fixed bugs.
The idea is to have an ultra performant game engine, capable of running on a potato.
Including support for many platforms.

### Building
Get scons and python, choose a platform from the platform directory and then run
`scons platform=name target=release tools=no -j#` (Replace "name" with the desired platform)(Replace # in "-j#" with your processors core count).

#### Linux
If you're building on Linux make sure to install libraries for alsa as well as opengl.

If you are on Fedora or a Fedora based distro you can install said required libraries with `sudo dnf install alsa-lib-devel glfw-devel`.
If you are on Debian or Debian based distro you can install said required libraries with `sudo apt install alsa-lib libglfw3-dev`.
Other Linux distros please refer to your package manager for an equivalent.
#### Windows
**IF YOU COMPILING FOR WINDOWS 95/98/ME, YOU MUST USE [OUR CUSTOM TOOLCHAIN](https://cdn.discordapp.com/attachments/1217468399039414302/1465942155494297793/fusion_toolchain.7z?ex=69ecfa45&is=69eba8c5&hm=fa43a709571c71ae0b62b4d208415cbba87c0c86a79f55a91cfe9d72f455769d&) ON WINDOWS OR [THIS TOOLCHAIN](https://github.com/ati9550/mingw-95/releases) ON LINUX**

#### Other platforms
Every console port uses their specific open source homebrew SDK.

### Making a game
Make a game in the editor.
The editor is unusual if you are familiar with Godot, but you'll be able to notice it's very intuitive after learning it.
Games can range from 3D to 2D to apps.
The main focus point of the engine has been the 3D part so far.

### Running a game
For all platforms so far, it's as simple as copying the data.pck (or project directory) into the same folder as the fusion engine binary.

### Website
[Fusion Engine https://fusionengine.org](https://web.archive.org/web/20250504132244/https://fusionengine.org/)

### Discord
[Homebrodot Discord Server](https://discord.gg/qfBhDqre9w)

[Wii Gamedev Discord Server](https://discord.gg/dWtSFYyhtg)
