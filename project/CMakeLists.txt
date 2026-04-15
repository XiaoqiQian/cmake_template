cmake_minimum_required(VERSION 3.16)
project(CefWrapperProject LANGUAGES CXX)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    # Release模式禁用优化，用于调试
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi /Od")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE
    "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
endif()

add_subdirectory(subprocess)
#add_subdirectory(wrapper)
#add_subdirectory(tests)

# 拷贝文件到输出目录
# file(COPY ${CMAKE_SOURCE_DIR}/3rdparty/cef/bin/ DESTINATION ${CMAKE_SOURCE_DIR}/output/bin/)
# file(COPY ${CMAKE_SOURCE_DIR}/config/ DESTINATION ${CMAKE_SOURCE_DIR}/output/bin/)

# # 查找Doxygen
# find_package(Doxygen REQUIRED)

# # 添加文档生成目标
# if(DOXYGEN_FOUND)
#     configure_file(${CMAKE_SOURCE_DIR}/cmake/Doxyfile.in Doxyfile @ONLY)
#     add_custom_target(Documentation
#         COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile
#         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
#         DEPENDS Doxyfile)
# endif()