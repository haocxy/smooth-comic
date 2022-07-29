cmake_minimum_required(VERSION 3.22.0 FATAL_ERROR)

set(appName smooth-comic)

project(${appName})

set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE BOTH)



set(myAppUseDebugConfig FALSE)

get_filename_component(buildDirName ${CMAKE_BINARY_DIR} NAME)

function(setMyAppDebugModeByDirName dirName outVar)
    if(${dirName} MATCHES ".*debug.*")
        set(${outVar} TRUE PARENT_SCOPE)
    endif()
    if()
        set(${outVar} FALSE PARENT_SCOPE)
    endif()
endfunction()

setMyAppDebugModeByDirName(${buildDirName} myAppUseDebugConfig)

if(${myAppUseDebugConfig})
    set(myAppUseReleaseConfig FALSE)
else()
    set(myAppUseReleaseConfig TRUE)
endif()

set(CMAKE_BUILD_TYPE Release)
set(prepareDir ${CMAKE_SOURCE_DIR}/prepare/release)
set(prepareScriptConfigArg release)
if(myAppUseDebugConfig)
    set(CMAKE_BUILD_TYPE Debug)
    set(prepareDir ${CMAKE_SOURCE_DIR}/prepare/debug)
    set(prepareScriptConfigArg debug)
endif()

if(ANDROID)
    message("=====> for ANDROID: CMAKE_TOOLCHAIN_FILE: ${CMAKE_TOOLCHAIN_FILE}")
    message("=====> for ANDROID: ANDROID_SDK_ROOT: ${ANDROID_SDK_ROOT}")
    message("=====> for ANDROID: ANDROID_ABI: ${ANDROID_ABI}")
    message("=====> for ANDROID: ANDROID_PLATFORM: ${ANDROID_PLATFORM}")
    message("=====> for ANDROID: ANDROID_NATIVE_API_LEVEL: ${ANDROID_NATIVE_API_LEVEL}")
    message("=====> for ANDROID: CMAKE_COMMAND: ${CMAKE_COMMAND}")
    message("=====> for ANDROID: CMAKE_GENERATOR: ${CMAKE_GENERATOR}")
    message("=====> for ANDROID: CMAKE_MAKE_PROGRAM: ${CMAKE_MAKE_PROGRAM}")
    set(prepareDir ${CMAKE_SOURCE_DIR}/prepare/android__${ANDROID_NATIVE_API_LEVEL}__${ANDROID_ABI}__${CMAKE_BUILD_TYPE})
endif()


message(STATUS "prepareDir: ${prepareDir}")

# 自己实现的 CMake 工具
include(CMake/find_source_files.cmake)
include(CMake/git_tools.cmake)

# 全局项目构建配置
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_FIND_LIBRARY_PREFIXES "lib;")

# 准备第三方库
find_package(Python3 REQUIRED)
message(STATUS "Python3 Executable: [${Python3_EXECUTABLE}]")

message(STATUS "Preparing third libs")
if(ANDROID)
    execute_process(
        COMMAND ${Python3_EXECUTABLE} tools/prepare.py
            --cmake_command ${CMAKE_COMMAND}
            --cmake_generator ${CMAKE_GENERATOR}
            --cmake_make_program ${CMAKE_MAKE_PROGRAM}
            -s ${CMAKE_SOURCE_DIR}
            -p ${prepareDir}
            -r ${CMAKE_SOURCE_DIR}/prepare/repo
            -c ${prepareScriptConfigArg}
            --cmake_toolchain_file ${CMAKE_TOOLCHAIN_FILE}
            --is_for_android true
            --android_abi ${ANDROID_ABI}
            --android_platform ${ANDROID_NATIVE_API_LEVEL}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
else()
    execute_process(
        COMMAND ${Python3_EXECUTABLE} tools/prepare.py --cmake_command ${CMAKE_COMMAND} -s ${CMAKE_SOURCE_DIR} -p ${prepareDir} -r ${CMAKE_SOURCE_DIR}/prepare/repo -c ${prepareScriptConfigArg}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
endif()

# 设置第三方库搜索路径
list(APPEND CMAKE_PREFIX_PATH ${prepareDir}/install/zlib)
list(APPEND CMAKE_PREFIX_PATH ${prepareDir}/install/liblzma)
list(APPEND CMAKE_PREFIX_PATH ${prepareDir}/install/libarchive)

include_directories(${prepareDir}/source/boost/)



# 查找第三方库
set(libQt Qt6)
find_package(${libQt} REQUIRED COMPONENTS Core Widgets Qml Quick QuickWidgets)
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

# 更新Qt翻译文件(.ts)和发布文件(.qm文件)
execute_process(
    COMMAND "lupdate" "src/gui" "-ts" "-no-obsolete" "trans/trans.zh_CN.ts"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
execute_process(
    COMMAND "lrelease" "trans/trans.zh_CN.ts" "-qm" "trans/trans.zh_CN.qm"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# add_subdirectory(third EXCLUDE_FROM_ALL)

# include_directories(
#     third/libarchive-3.6.1/libarchive
# )

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

if(${myAppUseReleaseConfig} AND MSVC)
    file(COPY "${CMAKE_SOURCE_DIR}/tools/win32/pack.bat" DESTINATION ${CMAKE_BINARY_DIR})
    file(COPY "${CMAKE_SOURCE_DIR}/tools/win32/repack.bat" DESTINATION ${CMAKE_BINARY_DIR})
    file(COPY "${CMAKE_SOURCE_DIR}/tools/win32/build.bat" DESTINATION ${CMAKE_BINARY_DIR})
    file(COPY "${CMAKE_SOURCE_DIR}/tools/win32/rebuild.bat" DESTINATION ${CMAKE_BINARY_DIR})
endif()

if(${myAppUseReleaseConfig} AND MSVC)
    getVersionPartByGit(packVersion)
    message(STATUS "Version Part By Git: [${packVersion}]")
    include(pack_win32.cmake)
endif()
