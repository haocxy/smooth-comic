#file(GLOB_RECURSE asset_files "assets/*")
#install(FILES ${asset_files} DESTINATION "assets")

install(TARGETS ${appName} ${appName}-cmd
    CONFIGURATIONS Release
    RUNTIME DESTINATION ".")

function(findPreparedDLL libName outvar)
    find_file(pathOfLib "${libName}"
        PATH_SUFFIXES "bin"
        NO_CACHE
    )
    if(NOT pathOfLib)
        message(FATAL_ERROR "Cannot find Prepared DLL: ${libName}")
    endif()
    message(STATUS "Prepared DLL [${libName}] Found: [${pathOfLib}]")
    set(${outvar} ${pathOfLib} PARENT_SCOPE)
endfunction()

findPreparedDLL("zlib.dll" dll_zlib)
install(FILES ${dll_zlib} DESTINATION ".")

findPreparedDLL("liblzma.dll" dll_liblzma)
install(FILES ${dll_liblzma} DESTINATION ".")

findPreparedDLL("archive.dll" dll_archive)
install(FILES ${dll_archive} DESTINATION ".")



set(CPACK_GENERATOR ZIP)
set(CPACK_PACKAGE_NAME ${appName})
set(CPACK_PACKAGE_VERSION ${packVersion})
include(CPack)
