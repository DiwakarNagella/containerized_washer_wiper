cmake_minimum_required(VERSION 3.2)
project(WasherWiperApplication)


file(GLOB WASHER_SRC "*.cpp")
add_executable(WasherWiperApplication ${WASHER_SRC}  )
# target_link_libraries (WasherWiperApplication  /opt/Xilinx/SDK/2017.1/gnu/aarch64/lin/aarch64-linux/aarch64-linux-gnu/lib64/libstdc++.so.6 -lpthread)

target_link_libraries (WasherWiperApplication  -lpthread)
