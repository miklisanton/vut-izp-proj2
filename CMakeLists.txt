cmake_minimum_required(VERSION 3.20)
project(IZP_proj_2 C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")
add_executable(IZP_proj_2 setcal.c)
