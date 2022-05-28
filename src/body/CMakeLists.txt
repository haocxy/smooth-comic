findSourceFiles(src_files)
add_library(${appName}-body STATIC ${src_files})
target_link_libraries(${appName}-body
    ${appName}-core
    Qt6::Core
    Qt6::Widgets
)


