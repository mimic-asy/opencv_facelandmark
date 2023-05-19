cmake_minimum_required(VERSION 2.8)
project( IVideoCapture )

add_compile_options(-g)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( IVideoCapture facelandmark.cpp )
target_link_libraries( IVideoCapture ${OpenCV_LIBS} )