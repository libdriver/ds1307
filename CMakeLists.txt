cmake_minimum_required(VERSION 3.12)

set(DS1307_INCLUDE src CACHE INTERNAL "" FORCE)
file(GLOB_RECURSE DS1307_SOURCES src/*.c)

include_directories(${DS1307_INCLUDE})
add_library(ds1307 STATIC ${DS1307_SOURCES})
target_include_directories(ds1307 SYSTEM PUBLIC ${DS1307_INCLUDE})
