set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# 使用 Qt UIC 生成C++文件
# 不使用 cmake 自带的 CMAKE_AUTOUIC 功能,
# 因为 cmake 自带的 CMAKE_AUTOUIC 直到构建才生成C++文件,
# 使用IDE开发时很麻烦
find_program(quic_exe uic)
if(${quic_exe} STREQUAL "quic_exe-NOTFOUND")
    message(FATAL_ERROR "Cannot find Qt uic executable")
endif()
message(STATUS "Qt uic executable found: ${quic_exe}")

message(STATUS "Updating Qt UIC")
execute_process(
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/tools/update_qt_uic.py" "${quic_exe}" "${CMAKE_CURRENT_SOURCE_DIR}"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)
message(STATUS "Qt UIC updated")

include_directories(.)
include_directories(${CMAKE_SOURCE_DIR}/src/body)

findSourceFiles(src_files)

add_library(${appName}-gui STATIC ${src_files} ${CMAKE_SOURCE_DIR}/resources.qrc qml/qml.qrc)

target_link_libraries(${appName}-gui
    ${appName}-util
    ${libQt}::Widgets
    ${libQt}::Qml
    ${libQt}::Quick
    ${libQt}::QuickWidgets
)

