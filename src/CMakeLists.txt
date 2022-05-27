findSourceFiles(src_files)
add_executable(${appName} ${src_files})
target_link_libraries(${appName}
    Qt6::Core
    Qt6::Widgets
)
