cmake_minimum_required(VERSION 3.5)

project(gblan9252_soes_config C)

file(GLOB SOURCES "src/*.c")

add_library(gblan9252_soes_config INTERFACE)
target_include_directories(gblan9252_soes_config INTERFACE inc ${gblan9252_soes_config_BINARY_DIR})