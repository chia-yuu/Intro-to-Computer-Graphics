# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.31

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

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = "C:/Program Files/CMake/bin/cmake.exe"

# The command to remove a file.
RM = "C:/Program Files/CMake/bin/cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:/ICG/final

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:/ICG/final/build

# Utility rule file for update_mappings.

# Include any custom commands dependencies for this target.
include extern/glfw/src/CMakeFiles/update_mappings.dir/compiler_depend.make

# Include the progress variables for this target.
include extern/glfw/src/CMakeFiles/update_mappings.dir/progress.make

extern/glfw/src/CMakeFiles/update_mappings:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=D:/ICG/final/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Updating gamepad mappings from upstream repository"
	cd D:/ICG/final/extern/glfw/src && "C:/Program Files/CMake/bin/cmake.exe" -P D:/ICG/final/extern/glfw/CMake/GenerateMappings.cmake mappings.h.in mappings.h

extern/glfw/src/CMakeFiles/update_mappings.dir/codegen:
.PHONY : extern/glfw/src/CMakeFiles/update_mappings.dir/codegen

update_mappings: extern/glfw/src/CMakeFiles/update_mappings
update_mappings: extern/glfw/src/CMakeFiles/update_mappings.dir/build.make
.PHONY : update_mappings

# Rule to build all files generated by this target.
extern/glfw/src/CMakeFiles/update_mappings.dir/build: update_mappings
.PHONY : extern/glfw/src/CMakeFiles/update_mappings.dir/build

extern/glfw/src/CMakeFiles/update_mappings.dir/clean:
	cd D:/ICG/final/build/extern/glfw/src && $(CMAKE_COMMAND) -P CMakeFiles/update_mappings.dir/cmake_clean.cmake
.PHONY : extern/glfw/src/CMakeFiles/update_mappings.dir/clean

extern/glfw/src/CMakeFiles/update_mappings.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" D:/ICG/final D:/ICG/final/extern/glfw/src D:/ICG/final/build D:/ICG/final/build/extern/glfw/src D:/ICG/final/build/extern/glfw/src/CMakeFiles/update_mappings.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : extern/glfw/src/CMakeFiles/update_mappings.dir/depend

