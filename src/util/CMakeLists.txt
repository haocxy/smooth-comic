set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

include_directories(.)

findSourceFiles(src_files)

add_library(${appName}-util STATIC ${src_files})

target_link_libraries(${appName}-util
    ${appName}-core
    archive_static
    ${libQt}::Core
    ${libQt}::Gui
)
