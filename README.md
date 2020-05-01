# AnimationPractice

## Developer setup
- Clone this repo
- Ensure you have a C++ compiler installed - I have used the Visual Studio 2019 Visual C++ compiler
- Download the zip of the SDL2 development library from [here](https://www.libsdl.org/download-2.0.php) and extract it to a convenient location on your machine
- Download the zip of the SDL2_image development library from [here](https://www.libsdl.org/projects/SDL_image/)
- Extract the contents of the include and lib folders from SDL2_image into where you extracted the include and lib folders of the SDL2 library
- On Windows, create an environment variable called SDL2 containing the parent directory of the include and lib folders directory.
- Install the latest release of CMake from [here](https://cmake.org/download/), accepting the default installation options
- Run Cmake and input the following options:
  - Where is the source code: \<this repo\>
  - Where to build the binaries: \<this repo\>/build
- Press configure and if prompted to create the build directory, click yes
- Choose your C++ compiler, press Finish and then press Generate
- Build the solution
- Once the solution has built, copy the SDL2.dll into the build directory, adjacent to the .exe file (for example \<this repo\>\bin\Debug when building in Debug mode using Visual Studio)
