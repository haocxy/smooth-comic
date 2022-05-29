set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

include_directories(.)

findSourceFiles(src_files)

add_library(${appName}-body STATIC ${src_files})

target_link_libraries(${appName}-body
    ${appName}-core
    LibArchive::LibArchive
    Qt6::Core
    Qt6::Widgets
)


