cmake_minimum_required(VERSION 3.7)
project(udpflow_v C)

set(CMAKE_C_STANDARD 99)
include(FindPCAP.cmake)

add_executable(UdpFlow main.c errors.h pcap_processing/wrappers/pcap_wrapper.c pcap_processing/wrappers/pcap_wrapper.h pcap_processing/wrappers/netinet_wrapper.c pcap_processing/wrappers/netinet_wrapper.h orchestrator.c orchestrator.h pcap_processing/data_types/session_metadata.c pcap_processing/data_types/session_metadata.h common.h pcap_processing/data_types/session_data.c pcap_processing/data_types/session_data.h pcap_processing/output_methods/json_output.c pcap_processing/output_methods/json_output.h pcap_processing/output_methods/file_operations.c pcap_processing/output_methods/file_operations.h pcap_processing/data_types/vlan.h)
target_link_libraries(UdpFlow ${PCAP_LIBRARY})