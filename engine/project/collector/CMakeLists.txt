set(PROJECT_NAME	collector)
project(${PROJECT_NAME})

AUX_SOURCE_DIRECTORY(src/decode SRC_DECODE_FILES)

set(SRC_FILES 		src/main.cpp
					${WIRESHARK_SRC}/frame_tvbuff.c
					${WIRESHARK_SRC}/cfile.c
					${SRC_DECODE_FILES})
set(SRC_HEADER		src/main.h)			

set(LIBS_PROJECT	common)

set(LIBS_3rdParty	wiretap
					wireshark					
					wsutil
					glib-2.0
					log4cplus
					ndpi
					aio
					${SYSTEM_LIBS})
					
set(LIBS_DPDK		-Wl,--whole-archive
					rte_ethdev
					rte_acl
					rte_pmd_af_packet
					rte_cfgfile
					rte_cmdline
					rte_distributor
					rte_eal
					rte_hash
					rte_ip_frag
					rte_jobstats
					rte_kni
					rte_kvargs
					rte_lpm
					rte_mbuf
					rte_mempool
					rte_meter
					rte_net
					rte_pipeline
					rte_pmd_bond
					rte_pmd_e1000
					rte_pmd_enic
					rte_pmd_fm10k
					rte_pmd_i40e
					rte_pmd_ixgbe
					rte_pmd_null
					rte_pmd_ring
					rte_pmd_virtio
					rte_pmd_vmxnet3_uio
					rte_port
					rte_power
					rte_reorder
					rte_ring
					rte_sched
					rte_table
					rte_timer	
					-Wl,--no-whole-archive)

message(STATUS "CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_C_FLAGS = ${CMAKE_C_FLAGS}")


#
# the include and link folders
#
include_directories("${RTE_SDK_BIN}/include")
include_directories("${RTE_SDK}/lib/librte_eal/common/include")
include_directories("${WIRESHARK_SRC}/wiretap")

link_directories("${RTE_SDK_BIN}/lib")
		
		
#
# config the target
#		
add_executable(${PROJECT_NAME} 	${SRC_FILES} 
								${SRC_HEADER})			# set the source files

target_link_libraries(${PROJECT_NAME} ${LIBS_PROJECT} ${LIBS_DPDK} ${LIBS_3rdParty})

set_target_properties (${PROJECT_NAME} PROPERTIES FOLDER app)	# set the project under folder app

