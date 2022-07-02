
function(getAllGitTagRefNames outvar)
    execute_process(
        COMMAND git tag -l "--format=%(refname)"
        OUTPUT_VARIABLE cmdout
    )
    string(REGEX REPLACE "[ \r\n]+" ";" tagrefnames "${cmdout}")
    set(${outvar} ${tagrefnames} PARENT_SCOPE)
endfunction()

function(getGitObjIdByRefName refname outvar)
    execute_process(
        COMMAND git show "${refname}" "-s" "--format=%H"
        OUTPUT_VARIABLE cmdout
    )
    string(STRIP ${cmdout} obj_id)
    set(${outvar} ${obj_id} PARENT_SCOPE)
endfunction()

function(getVersionPartByGit outvar)
    getAllGitTagRefNames(tagrefnames)
    getGitObjIdByRefName(HEAD head_obj_id)
    foreach(tagrefname ${tagrefnames})
        string(STRIP ${tagrefname} tagrefname)
        if(NOT(${tagrefname} MATCHES "refs/tags/v[0-9]+\\.[0-9]+\\.[0-9]+"))
            continue()
        endif()
        getGitObjIdByRefName(${tagrefname} tag_obj_id)
        if(${head_obj_id} STREQUAL ${tag_obj_id})
            string(REGEX REPLACE "^refs/tags/v" "" short_ref_name ${tagrefname})
            set(${outvar} ${short_ref_name} PARENT_SCOPE)
            return()
        endif()
    endforeach()
    set(${outvar} ${head_obj_id} PARENT_SCOPE)
endfunction()
