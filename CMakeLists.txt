cmake_minimum_required(VERSION 3.5)

project(ImgView VERSION 0.1 LANGUAGES CXX)

if(CMAKE_BUILD_TYPE STREQUAL "")
	set(CMAKE_BUILD_TYPE Debug)
	message(STATUS "Build type: Debug.")
else()
	message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
endif()

include_directories(${CMAKE_CURRENT_LIST_DIR}/src)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6Widgets REQUIRED)
find_package(OpenCV REQUIRED)

file(GLOB_RECURSE _src_file "src/*.cc")
file(GLOB_RECURSE _inc_file "src/*.h")
set(_qrc_file "img_view.qrc")

list(APPEND _qt_lib Qt6::Widgets)
list(APPEND _opencv_lib opencv_core opencv_imgcodecs opencv_imgproc)

add_executable(ImgView ${_src_file} ${_inc_file} ${_qrc_file})

target_link_libraries(ImgView PRIVATE ${_qt_lib} ${_opencv_lib})
