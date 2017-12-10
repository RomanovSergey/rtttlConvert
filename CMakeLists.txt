
cmake_minimum_required(VERSION 2.8)

project(rtttlConvert)

add_definitions(-Wall -g)

include_directories(src)

aux_source_directory(./src SRC_FILES)

add_executable(${PROJECT_NAME} ${SRC_FILES})

# target_link_libraries(${PROJECT_NAME} udev)

