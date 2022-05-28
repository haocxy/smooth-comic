set(mod ${appName}-core)
findSourceFiles(src_files)
add_library(${mod} STATIC ${src_files})
