set(mod ${appName}-loader)
findSourceFiles(src_files)
add_library(${mod} STATIC ${src_files})
target_link_libraries(${mod} ${appName}-core ${appName}-body-static)
