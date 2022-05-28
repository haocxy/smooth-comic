findSourceFiles(src_files)
add_library(${appName}-body-static STATIC ${src_files})
target_link_libraries(${appName}-body-static ${appName}-body)
