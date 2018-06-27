# Compile steps

- Install git (https://git-scm.com) and CMake 3.9.0 or higher (https://cmake.org)
	- Ensure they are added to your *PATH* environment variable
- Run the following commands in the terminal/command line:
	- `git clone https://github.com/ivalduan/bsf.git`
	- `git clone https://github.com/ivalduan/bsfLeap.git`
	- `cd bsfLeap`
	- `mkdir Build`
	- `cd Build`
	- `cmake -G "$generator$" ../`
		- Where *$generator$* should be replaced with any of the supported generators. Some common ones:
			- `Visual Studio 15 2017 Win64` - Visual Studio 2017 (64-bit build)
			- `Unix Makefiles`
			- `Ninja`
			- `Xcode`
			- See all valid generators: [cmake-generators](https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html)
- Build the project using your chosen tool
	- Build files are in the `bsfLeap/Build` folder
- Run the examples
	- Build with BUILD_LEAP_EXAMPLES ON
	- Example binaries are placed in the `bsfLeap/bin` folder

# Examples
* Physics - Demonstrates the use of variety of physics related components, including a character controller, rigidbodies and colliders.
