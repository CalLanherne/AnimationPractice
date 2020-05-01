cmake_minimum_required(VERSION 2.6)
project(AnimationTest)

# Use our modified FindSDL2* modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${AnimationTest_SOURCE_DIR}/cmake")
set(BIN_DIR ${AnimationTest_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

# Bump up warning levels appropriately for clang, gcc & msvc and build in debug mode
if (MSVC)
	if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
		string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
	endif()
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++11")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -g")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -O2")
endif()

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})
include_directories(${SDL2_IMAGE_INCLUDE_DIR})
include_directories(include)

add_executable(AnimationTest src/main.cpp)
add_executable(KBTest src/testkbinput.cpp)
target_link_libraries(AnimationTest ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY})
target_link_libraries(KBTest ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY})
install(TARGETS AnimationTest RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/bin)
install(TARGETS KBTest RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/bin)

if (NOT ${PROJECT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
	file(COPY "${PROJECT_SOURCE_DIR}/res" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()