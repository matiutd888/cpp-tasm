# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.15

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2019.3.3\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2019.3.3\bin\cmake\win\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "C:\Users\Mateusz\Desktop\skrypty\iii sem\JNP\Zadania\Zadanie-4-nowy"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "C:\Users\Mateusz\Desktop\skrypty\iii sem\JNP\Zadania\Zadanie-4-nowy\cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/JNP_1_task_4.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/JNP_1_task_4.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/JNP_1_task_4.dir/flags.make

CMakeFiles/JNP_1_task_4.dir/computer_example.cc.obj: CMakeFiles/JNP_1_task_4.dir/flags.make
CMakeFiles/JNP_1_task_4.dir/computer_example.cc.obj: ../computer_example.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="C:\Users\Mateusz\Desktop\skrypty\iii sem\JNP\Zadania\Zadanie-4-nowy\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/JNP_1_task_4.dir/computer_example.cc.obj"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\JNP_1_task_4.dir\computer_example.cc.obj -c "C:\Users\Mateusz\Desktop\skrypty\iii sem\JNP\Zadania\Zadanie-4-nowy\computer_example.cc"

CMakeFiles/JNP_1_task_4.dir/computer_example.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/JNP_1_task_4.dir/computer_example.cc.i"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "C:\Users\Mateusz\Desktop\skrypty\iii sem\JNP\Zadania\Zadanie-4-nowy\computer_example.cc" > CMakeFiles\JNP_1_task_4.dir\computer_example.cc.i

CMakeFiles/JNP_1_task_4.dir/computer_example.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/JNP_1_task_4.dir/computer_example.cc.s"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "C:\Users\Mateusz\Desktop\skrypty\iii sem\JNP\Zadania\Zadanie-4-nowy\computer_example.cc" -o CMakeFiles\JNP_1_task_4.dir\computer_example.cc.s

# Object files for target JNP_1_task_4
JNP_1_task_4_OBJECTS = \
"CMakeFiles/JNP_1_task_4.dir/computer_example.cc.obj"

# External object files for target JNP_1_task_4
JNP_1_task_4_EXTERNAL_OBJECTS =

JNP_1_task_4.exe: CMakeFiles/JNP_1_task_4.dir/computer_example.cc.obj
JNP_1_task_4.exe: CMakeFiles/JNP_1_task_4.dir/build.make
JNP_1_task_4.exe: CMakeFiles/JNP_1_task_4.dir/linklibs.rsp
JNP_1_task_4.exe: CMakeFiles/JNP_1_task_4.dir/objects1.rsp
JNP_1_task_4.exe: CMakeFiles/JNP_1_task_4.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="C:\Users\Mateusz\Desktop\skrypty\iii sem\JNP\Zadania\Zadanie-4-nowy\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable JNP_1_task_4.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\JNP_1_task_4.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/JNP_1_task_4.dir/build: JNP_1_task_4.exe

.PHONY : CMakeFiles/JNP_1_task_4.dir/build

CMakeFiles/JNP_1_task_4.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\JNP_1_task_4.dir\cmake_clean.cmake
.PHONY : CMakeFiles/JNP_1_task_4.dir/clean

CMakeFiles/JNP_1_task_4.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "C:\Users\Mateusz\Desktop\skrypty\iii sem\JNP\Zadania\Zadanie-4-nowy" "C:\Users\Mateusz\Desktop\skrypty\iii sem\JNP\Zadania\Zadanie-4-nowy" "C:\Users\Mateusz\Desktop\skrypty\iii sem\JNP\Zadania\Zadanie-4-nowy\cmake-build-debug" "C:\Users\Mateusz\Desktop\skrypty\iii sem\JNP\Zadania\Zadanie-4-nowy\cmake-build-debug" "C:\Users\Mateusz\Desktop\skrypty\iii sem\JNP\Zadania\Zadanie-4-nowy\cmake-build-debug\CMakeFiles\JNP_1_task_4.dir\DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/JNP_1_task_4.dir/depend

