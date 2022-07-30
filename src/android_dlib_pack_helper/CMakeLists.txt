if(ANDROID)

    findSourceFiles(src_files)

    qt_add_executable(android-dlib-pack-helper ${src_files})



    add_library(stdcpp-shared SHARED IMPORTED)
    find_library(stdcpp-shared-libpath c++_shared)
    set_target_properties(stdcpp-shared PROPERTIES
        IMPORTED_LOCATION ${stdcpp-shared-libpath})
    target_link_libraries(android-dlib-pack-helper PRIVATE stdcpp-shared)
    if(NOT stdcpp-shared-libpath)
        message(FATAL_ERROR "for ANDROID: Cannot find [c++_shared]")
    endif()
    message("for ANDROID: stdcpp-shared found: ${stdcpp-shared-libpath}")

    add_library(myapp-liblzma SHARED IMPORTED)
    find_library(myapp-liblzma-libpath lzma)
    set_target_properties(myapp-liblzma PROPERTIES
        IMPORTED_LOCATION ${myapp-liblzma-libpath})
    target_link_libraries(android-dlib-pack-helper PRIVATE myapp-liblzma)
    if(NOT myapp-liblzma-libpath)
        message(FATAL_ERROR "for ANDROID: Cannot find [lzma]")
    endif()
    message("for ANDROID: myapp-liblzma found: ${myapp-liblzma-libpath}")

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

        target_link_libraries(android-dlib-pack-helper PRIVATE ${libname})

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