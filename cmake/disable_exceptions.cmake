function(disable_exception_support targetName)
    if (MSVC)
        target_compile_options(${targetName} PRIVATE /EHs-c-)
    elseif (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGXX)
        target_compile_options(${targetName} PRIVATE -fno-exceptions)
    endif()
endfunction() 


######################################################################



function(enable_exception_support targetName)
    if (MSVC)
        target_compile_options(${targetName} PRIVATE /EHsc)
    elseif (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGXX)
        #target_compile_options(${targetName} PRIVATE -f-exceptions)
    endif()
endfunction() 



if (MSVC)
    message("Removing exception handling parameter from CXX flags")
    string(REGEX REPLACE "/EHsc" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif()
