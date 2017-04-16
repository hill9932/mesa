cmake_minimum_required(VERSION 2.8.4)
project(engine)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_VERBOSE_MAKEFILE 		ON)
set(CMAKE_INCLUDE_CURRENT_DIR 	ON)

set(PROJECT_ROOT_PATH 		${CMAKE_CURRENT_SOURCE_DIR})
set(3RDPARTY_LIB_DIR 		"3rdParty/lib")		# record where to find the 3rdParty libraries
set(SYSTEM_LOCAL_LIB_PATH	"/usr/lib/x86_64-linux-gnu")
set(SYSTEM_LIBS 			"")
set(EXECUTABLE_OUTPUT_PATH 	"${PROJECT_ROOT_PATH}/bin")
set(LIBRARY_OUTPUT_PATH 	"${PROJECT_ROOT_PATH}/lib")

#
# set up the DPDK environment
#
if ($ENV{RTE_SDK})
    set(RTE_SDK             $ENV{RTE_SDK})
else()
	set(RTE_SDK             ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/dpdk)
endif()

set(RTE_TARGET				"x86_64-native-linuxapp-gcc")
set(RTE_SDK_BIN				${RTE_SDK}/${RTE_TARGET})
set(WIRESHARK_SRC			"${PROJECT_ROOT_PATH}/3rdParty/wireshark")			

message(STATUS "RTE_SDK_BIN path：${RTE_SDK_BIN}")

if (NOT RTE_SDK)
	message(FATAL_ERROR "Please set RTE_SDK.")
endif(NOT RTE_SDK)

#
# compiler settings
#
add_compile_options(
	-W 
	-Wall
	-Wunused
	-Wunused-variable 
	-Wunused-parameter 
	-Wunused-function 
	-Wno-deprecated 
)

add_definitions(
	-DTIXML_USE_STL
	-DLOG_FILE_LINE_INFORMATION
)

if (CMAKE_COMPILER_IS_GNUCXX)	
	add_definitions(-DLINUX)
	
	add_compile_options(
		-Woverloaded-virtual 
		-Wwrite-strings
		-ggdb 
		-std=c++11 
		-ffunction-sections
		-msse4.2
	)
	
	set(SYSTEM_LIBS rt m dl pthread)
	
else()	# MSVC
    add_definitions(-DWIN32)
	add_definitions(-DWIN32_LEAN_AND_MEAN)	

	add_definitions(/wd"4244" /wd"4267" /wd"4996" /wd"4800")
	
	set(CMAKE_EXE_LINKER_FLAGS	"${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:\"LIBCMT\"")
	
endif()

#
# the include and link folders
#
include_directories("${PROJECT_ROOT_PATH}/3rdParty")
include_directories("${PROJECT_ROOT_PATH}/3rdParty/boost")
include_directories("${PROJECT_ROOT_PATH}/3rdParty/lua/src")
include_directories("${PROJECT_ROOT_PATH}/3rdParty/gtest/include")
include_directories("${PROJECT_ROOT_PATH}/3rdParty/log4cplus/include")
include_directories("${PROJECT_ROOT_PATH}/3rdParty/tbb/include")
include_directories("${PROJECT_ROOT_PATH}/3rdParty/dpdk")
include_directories("${PROJECT_ROOT_PATH}/3rdParty/glib")
include_directories("${PROJECT_ROOT_PATH}/3rdParty/wireshark")
include_directories("${PROJECT_ROOT_PATH}/3rdParty/ndpi/src/include")
include_directories("${SYSTEM_LOCAL_LIB_PATH}/glib-2.0/include")
include_directories("${PROJECT_ROOT_PATH}/share/common/src")
include_directories("${PROJECT_ROOT_PATH}/test/include")

link_directories("${PROJECT_ROOT_PATH}/${3RDPARTY_LIB_DIR}")
link_directories("${PROJECT_ROOT_PATH}/lib")


#
# include the subdirectories
#
#add_subdirectory(${PROJECT_ROOT_PATH}/3rdParty/gtest)

add_subdirectory(share/common)
add_subdirectory(project/collector)


