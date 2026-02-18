
cmake_minimum_required (VERSION 3.5)

project(tracking)

find_package( OpenCV REQUIRED )
find_package( Boost REQUIRED COMPONENTS serialization )

include_directories(.)
link_directories ( ${Boost_LIBRARY_DIRS} )
include_directories ( ${Boost_INCLUDE_DIRS} )
link_directories ( ${OpenCV_LIBRARY_DIRS} )
include_directories ( ${OpenCV_INCLUDE_DIRS} )


FILE(GLOB sources ./*.cc )
	

					

SET(CMAKE_CXX_FLAGS "-O3 -Wall")

add_executable(track ${sources})
target_link_libraries(track ${OpenCV_LIBS} ${Boost_LIBRARIES})
