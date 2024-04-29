#cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
cmake_minimum_required(VERSION 3.11)
project(linux-kernel-development VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Variable m_target should be passed as an argument when calling cmake
set(m_target app CACHE STRING "Specify the m_target?")

add_subdirectory("${m_target}")