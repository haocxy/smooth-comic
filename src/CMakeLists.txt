# 为SQL文件等小型文件生成可直接使用的包装代码
include(assets.cmake)
remove_useless_files_generated_for_assets()
generate_src_file_for_all_assets()

# 查找 boost
if(MSVC)
    set(Boost_USE_STATIC_LIBS ON)
    set(Boost_USE_STATIC_RUNTIME OFF)
endif()

set(MY_BOOST_ROOT ${prepareDir}/install/boost)
set(MY_BOOST_VER "1.78")


if(${myAppUseDebugConfig})
    add_definitions(-DMYAPP_SOLUTION_ROOT_DIR=\"${CMAKE_SOURCE_DIR}\")
endif()


add_definitions(-DBOOST_DLL_USE_STD_FS)




include_directories(.)

# 模块功能：和业务无关的很常用的逻辑，例如字符串处理等
# 特点：很小的静态库
# 注意：这个模块的二进制产物应该很小，最好多使用内联
add_subdirectory(core)

# 模块功能：和业务有一定关系的公用逻辑，例如压缩包的支持
# 特点：不需要关注大小的静态库
# 注意：注意区分core模块，core模块可能会被loader模块使用，但util模块仅被body和gui使用
#       所以，core模块必须尽可能小，但util可以较大
add_subdirectory(util)

# 模块功能：主要的业务逻辑，项目要实现的具体功能都在这个模块中
# 特点：很大的静态库
add_subdirectory(body)

# 模块功能：GUI逻辑
add_subdirectory(gui)

# 模块功能：没有任何逻辑，仅用于把 body 模块包装为动态库
add_subdirectory(body-dynamic)

# 模块功能：没有任何逻辑，仅用于把 body 模块包装为静态库
# 实际上，使用 body-static 和直接使用 body 是一样的，这样做是为了明确模块功能
add_subdirectory(body-static)

# 模块功能：
# 1：根据命令行参数决定是否为单例模式
# 2：若为单例模式则加载 body-dynamic 或 body-static
# 模块目的：
#  ：提升程序启动速度，当程序以单例模式运行时可以免去加载过多的库
# 特点：
#  ：较小的静态库
add_subdirectory(loader)

# 模块功能：提供程序入口，即提供可执行文件
add_subdirectory(entry)

# 配合Android的构建工具，把需要动态加载的DLL都挂在这个项目下
add_subdirectory(android_dlib_pack_helper)
