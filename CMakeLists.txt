cmake_minimum_required(VERSION 3.14.0 FATAL_ERROR)

set(appName smooth-comic)

project(${appName})

# 自己实现的 CMake 工具
include(CMake/find_source_files.cmake)

# 全局项目构建配置
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_FIND_LIBRARY_PREFIXES "lib;")

# 准备第三方库
find_package(Python3 REQUIRED)
message(STATUS "Python3 Executable: [${Python3_EXECUTABLE}]")

message(STATUS "Preparing third libs")
execute_process(
    COMMAND ${Python3_EXECUTABLE} tools/prepare.py ${CMAKE_SOURCE_DIR}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# 设置第三方库搜索路径
set(prepareDir ${CMAKE_SOURCE_DIR}/prepare)
list(APPEND CMAKE_PREFIX_PATH ${prepareDir}/libarchive/install)

# 查找第三方库
set(libQt Qt6)
find_package(${libQt} REQUIRED COMPONENTS Core Widgets)
find_package(LibArchive REQUIRED)

# VS专用全局设置
if(MSVC)
    # 关闭有关源代码字符集的警告
    add_compile_options(/wd4819)
    # 关闭丢弃掉标有 [nodiscard] 的函数的返回值的编译警告
    add_compile_options(/wd4834)
    # 多处理器编译
    add_compile_options(/MP)
    # Debug配置启用AddressSanitizer技术
    #add_compile_options($<$<CONFIG:Debug>:/fsanitize=address>)
    # 使用静态运行库
    # 注意！必须根据配置选择 /MDd 或 /MD
    add_compile_options(/MD$<$<CONFIG:Debug>:d>)
    # 定义支持的最低windows版本(本项暂时定位面向win10及以上的windows版本)
    add_definitions(/D_WIN32_WINNT=0x0A00)
else()
    # 关闭丢弃掉标有 [nodiscard] 的函数的返回值的编译警告
    add_compile_options(-Wno-unused-result)
    add_link_options(-ldl)
endif()


# GNU体系全局设置
if(CMAKE_COMPILER_IS_GNUCXX)
    # 确保std::filesystem可用
    link_libraries(-lstdc++fs)
    message(STATUS "option: -lstdc++fs")
    link_libraries(-lpthread)
    message(STATUS "option: -lpthread")
endif()

add_subdirectory(src)

set_target_properties(
    ${appName}
    PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out$<CONFIG>"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out$<CONFIG>"
)

if(MSVC)
    set_target_properties(
        ${appName}-cmd
        PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out$<CONFIG>"
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out$<CONFIG>"
    )
endif()
