# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.18

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files (x86)\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files (x86)\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "C:\Users\mp725\OneDrive\Documents\Game Dev\C\TestingAnonymousCalls"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "C:\Users\mp725\OneDrive\Documents\Game Dev\C\TestingAnonymousCalls\build"

# Include any dependencies generated for this target.
include CMakeFiles/TestingCalls.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/TestingCalls.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/TestingCalls.dir/flags.make

CMakeFiles/TestingCalls.dir/src/main.c.obj: CMakeFiles/TestingCalls.dir/flags.make
CMakeFiles/TestingCalls.dir/src/main.c.obj: ../src/main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="C:\Users\mp725\OneDrive\Documents\Game Dev\C\TestingAnonymousCalls\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/TestingCalls.dir/src/main.c.obj"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\TestingCalls.dir\src\main.c.obj -c "C:\Users\mp725\OneDrive\Documents\Game Dev\C\TestingAnonymousCalls\src\main.c"

CMakeFiles/TestingCalls.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/TestingCalls.dir/src/main.c.i"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "C:\Users\mp725\OneDrive\Documents\Game Dev\C\TestingAnonymousCalls\src\main.c" > CMakeFiles\TestingCalls.dir\src\main.c.i

CMakeFiles/TestingCalls.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/TestingCalls.dir/src/main.c.s"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "C:\Users\mp725\OneDrive\Documents\Game Dev\C\TestingAnonymousCalls\src\main.c" -o CMakeFiles\TestingCalls.dir\src\main.c.s

# Object files for target TestingCalls
TestingCalls_OBJECTS = \
"CMakeFiles/TestingCalls.dir/src/main.c.obj"

# External object files for target TestingCalls
TestingCalls_EXTERNAL_OBJECTS =

TestingCalls.exe: CMakeFiles/TestingCalls.dir/src/main.c.obj
TestingCalls.exe: CMakeFiles/TestingCalls.dir/build.make
TestingCalls.exe: CMakeFiles/TestingCalls.dir/linklibs.rsp
TestingCalls.exe: CMakeFiles/TestingCalls.dir/objects1.rsp
TestingCalls.exe: CMakeFiles/TestingCalls.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="C:\Users\mp725\OneDrive\Documents\Game Dev\C\TestingAnonymousCalls\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable TestingCalls.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\TestingCalls.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/TestingCalls.dir/build: TestingCalls.exe

.PHONY : CMakeFiles/TestingCalls.dir/build

CMakeFiles/TestingCalls.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\TestingCalls.dir\cmake_clean.cmake
.PHONY : CMakeFiles/TestingCalls.dir/clean

CMakeFiles/TestingCalls.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "C:\Users\mp725\OneDrive\Documents\Game Dev\C\TestingAnonymousCalls" "C:\Users\mp725\OneDrive\Documents\Game Dev\C\TestingAnonymousCalls" "C:\Users\mp725\OneDrive\Documents\Game Dev\C\TestingAnonymousCalls\build" "C:\Users\mp725\OneDrive\Documents\Game Dev\C\TestingAnonymousCalls\build" "C:\Users\mp725\OneDrive\Documents\Game Dev\C\TestingAnonymousCalls\build\CMakeFiles\TestingCalls.dir\DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/TestingCalls.dir/depend
