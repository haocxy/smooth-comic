findSourceFiles(src_files)

list(APPEND deps ${appName}-loader)


# 所有平台都有的默认程序入口
add_executable(${appName} ${src_files})
target_link_libraries(${appName} ${deps})

# 如果是windows平台，则把默认入口设置为GUI方式启动
if(MSVC)
    target_link_options(${appName}
    # windows平台的入口为WinMain，默认的main是带命令行的
    PRIVATE "/SUBSYSTEM:WINDOWS"
    )
endif()

# 为windows平台增加命令行启动方式的程序，便于通过命令行调试
if(MSVC)
    add_executable(${appName}-cmd ${src_files})
    target_compile_definitions(${appName}-cmd PRIVATE SMOOTH_COMIC_WINDOWS_CONSOLE)
    target_link_libraries(${appName}-cmd ${deps})
endif()