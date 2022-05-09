cmake_minimum_required(VERSION 3.5)

project(ImgView VERSION 0.1 LANGUAGES CXX)

include_directories(${CMAKE_CURRENT_LIST_DIR}/src)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6Widgets CONFIG REQUIRED)

file(GLOB_RECURSE _src_file "src/*.cc")
file(GLOB_RECURSE _inc_file "src/*.h")
set(_qrc_file "img_view.qrc")

add_executable(ImgView ${_src_file} ${_inc_file} ${_qrc_file})

target_link_libraries(ImgView PRIVATE Qt::Widgets Qt6::Widgets Qt::WidgetsPrivate Qt6::WidgetsPrivate)
