include_directories(.)

findSourceFiles(src_files)

add_library(${appName}-body STATIC ${src_files})

target_link_libraries(${appName}-body
    ${appName}-util
    ${appName}-gui
)
