set(mod ${appName}-core)
findSourceFiles(src_files)
add_library(${mod} STATIC ${src_files})

find_package(Boost ${MY_BOOST_VER} REQUIRED HINTS ${MY_BOOST_ROOT})
target_link_libraries(${appName}-core Boost::boost)
