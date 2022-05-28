findSourceFiles(src_files)
add_library(${appName}-body-dynamic SHARED ${src_files})
target_link_libraries(${appName}-body-dynamic ${appName}-body)
