findSourceFiles(src_files)

if(MSVC)
    list(APPEND src_files ${CMAKE_SOURCE_DIR}/windows.rc)
endif()


list(APPEND deps ${appName}-loader)



# 所有平台都有的默认程序入口
qt_add_executable(${appName} ${src_files})
target_link_libraries(${appName} PRIVATE ${deps})


if(ANDROID)
    add_library(stdcpp-shared SHARED IMPORTED)
    find_library(stdcpp-shared-libpath c++_shared)
    set_target_properties(stdcpp-shared PROPERTIES
        IMPORTED_LOCATION ${stdcpp-shared-libpath})
    target_link_libraries(${appName} PRIVATE stdcpp-shared)
    message(STATUS "for ANDROID: stdcpp-shared found: ${stdcpp-shared-libpath}")

    function(addQtPluginDepForAndroid plugin_type_dir_name plugin_name)
        find_file(pathOfLib "libplugins_${plugin_type_dir_name}_${plugin_name}_${ANDROID_ABI}.so"
            PATH_SUFFIXES "plugins/${plugin_type_dir_name}"
            NO_CACHE
        )
        if(NOT pathOfLib)
            message(FATAL_ERROR "Cannot find QtPlugin [${plugin_name}] for [${ANDROID_ABI}]")
        endif()
        message("for ANDROID: Qt plugin [${plugin_name}] found: [${pathOfLib}]")
        set(libname myapp-qtplugin-${plugin_name})
        add_library(${libname} SHARED IMPORTED)
        set_target_properties(${libname} PROPERTIES
            IMPORTED_LOCATION ${pathOfLib}
        )
        message("for ANDROID: imported lib created: [${libname}] on [${pathOfLib}]")

        target_link_libraries(${appName} PRIVATE ${libname})

    endfunction()

    addQtPluginDepForAndroid(platforms qtforandroid)
    addQtPluginDepForAndroid(styles qandroidstyle)
    addQtPluginDepForAndroid(imageformats qgif)
    addQtPluginDepForAndroid(imageformats qicns)
    addQtPluginDepForAndroid(imageformats qico)
    addQtPluginDepForAndroid(imageformats qjpeg)
    addQtPluginDepForAndroid(imageformats qtga)
    addQtPluginDepForAndroid(imageformats qtiff)
    addQtPluginDepForAndroid(imageformats qwbmp)
    addQtPluginDepForAndroid(imageformats qwebp)

endif()


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

add_custom_command(TARGET ${appName} POST_BUILD
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/tools/update_output_dir.py ${prepareDir}/install $<TARGET_FILE_DIR:${appName}>
)
