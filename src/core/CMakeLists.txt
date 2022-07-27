set(mod ${appName}-core)
findSourceFiles(src_files)
add_library(${mod} STATIC ${src_files})

target_link_libraries(${appName}-core
    ${libQt}::Widgets
)
