function(isSrcForCurPlat out_var file_path)
    if(${file_path} MATCHES "^.+\\.win32\\.(h|cpp|c)")
        if(MSVC)
            set(${out_var} TRUE PARENT_SCOPE)
            return()
        else()
            set(${out_var} FALSE PARENT_SCOPE)
            return()
        endif()
    elseif(${file_path} MATCHES "^.+\\.unix\\.(h|cpp|c)")
        if(MSVC)
            set(${out_var} FALSE PARENT_SCOPE)
            return()
        else()
            set(${out_var} TRUE PARENT_SCOPE)
            return()
        endif()
    elseif(${file_path} MATCHES "^.+\\.(h|cpp|c)")
        set(${out_var} TRUE PARENT_SCOPE)
        return()
    else()
        set(${out_var} FALSE PARENT_SCOPE)
        return()
    endif()
endfunction()

function(findSourceFiles output_files)
    file(GLOB_RECURSE cpp_files
        "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
    )
    set(result "")
    foreach(cpp_file ${cpp_files})
        isSrcForCurPlat(should_add ${cpp_file})
        if(${should_add})
            list(APPEND result ${cpp_file})
        endif()
    endforeach()
    file(GLOB_RECURSE other_files
        "${CMAKE_CURRENT_SOURCE_DIR}/*.txt"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cmake"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.sql"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.ui"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.qml"
    )
    list(APPEND result ${other_files})
    source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/.." FILES ${result})
    set(${output_files} ${result} PARENT_SCOPE)
endfunction()


function(findFilesByExt output_files ext_name)
    file(GLOB_RECURSE files ${CMAKE_CURRENT_SOURCE_DIR}/*.${ext_name})
    set(output_files ${files} PARENT_SCOPE)
endfunction()
