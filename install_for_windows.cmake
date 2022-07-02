#file(GLOB_RECURSE asset_files "assets/*")
#install(FILES ${asset_files} DESTINATION "assets")

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

function(addQtPlugin libName pluginPath)
    find_file(pathOfLib "${libName}"
        PATH_SUFFIXES "plugins/${pluginPath}"
        NO_CACHE
    )
    if(NOT pathOfLib)
        message(FATAL_ERROR "Cannot find Qt Plugin: ${libName}")
    endif()
    message(STATUS "Qt Plugin [${libName}] Found: [${pathOfLib}]")
    install(FILES "${pathOfLib}" DESTINATION "${pluginPath}")
endfunction()

addPreparedDLL(zlib.dll)
addPreparedDLL(liblzma.dll)
addPreparedDLL(archive.dll)

addSystemDLL(Qt6Core.dll)
addSystemDLL(Qt6Gui.dll)
addSystemDLL(Qt6Widgets.dll)

addSystemDLL(MSVCP140.dll)
addSystemDLL(MSVCP140_1.dll)
addSystemDLL(MSVCP140_2.dll)
addSystemDLL(MSVCP140_ATOMIC_WAIT.dll)
addSystemDLL(VCRUNTIME140.dll)
addSystemDLL(VCRUNTIME140_1.dll)

addQtPlugin(qwindows.dll platforms)
addQtPlugin(qwindowsvistastyle.dll styles)

addQtPlugin(qgif.dll imageformats)
addQtPlugin(qicns.dll imageformats)
addQtPlugin(qico.dll imageformats)
addQtPlugin(qjpeg.dll imageformats)
addQtPlugin(qpdf.dll imageformats)
addQtPlugin(qsvg.dll imageformats)
addQtPlugin(qtga.dll imageformats)
addQtPlugin(qtiff.dll imageformats)
addQtPlugin(qwbmp.dll imageformats)
addQtPlugin(qwebp.dll imageformats)

set(CPACK_GENERATOR ZIP)
set(CPACK_PACKAGE_NAME ${appName})
set(CPACK_PACKAGE_VERSION ${packVersion})
include(CPack)
