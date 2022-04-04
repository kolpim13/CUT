cmake_minimum_required(VERSION 3.10)

# Choose compiler and flags. (Try to comment these lines if doesn`t work)
	# GCC
#SET(CMAKE_C_COMPILER "gcc-9.4.0")
SET (CMAKE_C_FLAGS, "-Wall -Wextra, -std=c11")

	# CLANG
#SET(CMAKE_C_COMPILER, "usr/bin/clang")


# Project name
project(CUT) 

# Regular vars
set(path_src ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(path_inc ${CMAKE_CURRENT_SOURCE_DIR}/src/inc)
set(source 
    ${path_src}/main.c ${path_inc}/main.h
	${path_src}/reader.c ${path_inc}/reader.h
	${path_src}/analyzer.c ${path_inc}/analyzer.h
	${path_src}/printer.c ${path_inc}/printer.h
	${path_src}/general.c ${path_inc}/general.h)
  
# Add the executables
add_executable(program ${source})

# Add pthread library
set(THREADS_PREFER_PTHREAD_INIT true)
set(THREADS_PREFER_PTHREAD_INIT ON)
find_package(Threads REQUIRED)
target_link_libraries(program Threads::Threads)

# Add headers
target_include_directories(program PRIVATE ${path_inc})

# Choose C standart (C11)
#set_property(TARGET program PROPERTY C_STANDART 11)
