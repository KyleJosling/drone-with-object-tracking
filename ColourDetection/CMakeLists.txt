cmake_minimum_required(VERSION 2.8)

if (POLICY CMP0048)
    cmake_policy(SET CMP0048 NEW)
endif(POLICY CMP0048)


#Compile as C++11
add_compile_options(-std=c++11)

project(Object-Tracking-Drone VERSION 0.1.0)

include_directories(include)
add_subdirectory(src)

#Preprocessor macros
add_definitions(src DISPLAY)
#add_definitions(src FLIGHT_CONTROLLER)

install(DIRECTORY include/ DESTINATION include)
