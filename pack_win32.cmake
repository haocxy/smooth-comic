install(DIRECTORY "$<TARGET_FILE_DIR:${appName}>/" DESTINATION ".")

file(GLOB_RECURSE style_files "styles/*")
install(FILES ${style_files} DESTINATION "styles")

install(TARGETS ${appName} ${appName}-cmd
    CONFIGURATIONS Release
    RUNTIME DESTINATION ".")

function(addSystemDLL libName)
    find_file(pathOfLib "${libName}"
        NO_CACHE
    )
    if(NOT pathOfLib)
        message(FATAL_ERROR "Cannot find System DLL: ${libName}")
    endif()
    message(STATUS "System DLL [${libName}] Found: [${pathOfLib}]")
    install(FILES "${pathOfLib}" DESTINATION ".")
endfunction()

function(addPreparedDLL libName)
    find_file(pathOfLib "${libName}"
        PATH_SUFFIXES "bin"
        NO_CACHE
    )
    if(NOT pathOfLib)
        message(FATAL_ERROR "Cannot find Prepared DLL: ${libName}")
    endif()
    message(STATUS "Prepared DLL [${libName}] Found: [${pathOfLib}]")
    install(FILES "${pathOfLib}" DESTINATION ".")
endfunction()

addPreparedDLL(zlib.dll)
addPreparedDLL(liblzma.dll)
addPreparedDLL(archive.dll)

addSystemDLL(MSVCP140.dll)
addSystemDLL(MSVCP140_1.dll)
addSystemDLL(MSVCP140_2.dll)
addSystemDLL(MSVCP140_ATOMIC_WAIT.dll)
addSystemDLL(VCRUNTIME140.dll)
addSystemDLL(VCRUNTIME140_1.dll)

set(CPACK_GENERATOR ZIP)
set(CPACK_PACKAGE_NAME ${appName})
set(CPACK_PACKAGE_VERSION ${packVersion})
include(CPack)
