# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.30

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
CMAKE_COMMAND = /home/melisa/.local/share/JetBrains/Toolbox/apps/clion/bin/cmake/linux/x64/bin/cmake

# The command to remove a file.
RM = /home/melisa/.local/share/JetBrains/Toolbox/apps/clion/bin/cmake/linux/x64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/melisa/CLionProjects/ReaderWriterProject

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/melisa/CLionProjects/ReaderWriterProject/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/ReadWriterProject.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/ReadWriterProject.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/ReadWriterProject.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ReadWriterProject.dir/flags.make

CMakeFiles/ReadWriterProject.dir/main.c.o: CMakeFiles/ReadWriterProject.dir/flags.make
CMakeFiles/ReadWriterProject.dir/main.c.o: /home/melisa/CLionProjects/ReaderWriterProject/main.c
CMakeFiles/ReadWriterProject.dir/main.c.o: CMakeFiles/ReadWriterProject.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/melisa/CLionProjects/ReaderWriterProject/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/ReadWriterProject.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/ReadWriterProject.dir/main.c.o -MF CMakeFiles/ReadWriterProject.dir/main.c.o.d -o CMakeFiles/ReadWriterProject.dir/main.c.o -c /home/melisa/CLionProjects/ReaderWriterProject/main.c

CMakeFiles/ReadWriterProject.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/ReadWriterProject.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/melisa/CLionProjects/ReaderWriterProject/main.c > CMakeFiles/ReadWriterProject.dir/main.c.i

CMakeFiles/ReadWriterProject.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/ReadWriterProject.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/melisa/CLionProjects/ReaderWriterProject/main.c -o CMakeFiles/ReadWriterProject.dir/main.c.s

# Object files for target ReadWriterProject
ReadWriterProject_OBJECTS = \
"CMakeFiles/ReadWriterProject.dir/main.c.o"

# External object files for target ReadWriterProject
ReadWriterProject_EXTERNAL_OBJECTS =

ReadWriterProject: CMakeFiles/ReadWriterProject.dir/main.c.o
ReadWriterProject: CMakeFiles/ReadWriterProject.dir/build.make
ReadWriterProject: CMakeFiles/ReadWriterProject.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/melisa/CLionProjects/ReaderWriterProject/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable ReadWriterProject"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ReadWriterProject.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ReadWriterProject.dir/build: ReadWriterProject
.PHONY : CMakeFiles/ReadWriterProject.dir/build

CMakeFiles/ReadWriterProject.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ReadWriterProject.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ReadWriterProject.dir/clean

CMakeFiles/ReadWriterProject.dir/depend:
	cd /home/melisa/CLionProjects/ReaderWriterProject/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/melisa/CLionProjects/ReaderWriterProject /home/melisa/CLionProjects/ReaderWriterProject /home/melisa/CLionProjects/ReaderWriterProject/cmake-build-debug /home/melisa/CLionProjects/ReaderWriterProject/cmake-build-debug /home/melisa/CLionProjects/ReaderWriterProject/cmake-build-debug/CMakeFiles/ReadWriterProject.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/ReadWriterProject.dir/depend

