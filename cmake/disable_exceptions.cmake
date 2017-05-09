

function(disable_exception_support_msvc targetName)
    # FIXME: compiler command line warning:  D9025: "/EHs" "/EHs-"
    target_compile_options(${targetName} PRIVATE /EHs-c-)
endfunction()


function(disable_exception_support_gcc targetName)
    target_compile_options(${targetName} PRIVATE -fno-exceptions)
endfunction()






function(disable_exception_support targetName)

if (MSVC)
    disable_exception_support_msvc(${targetName})
elseif (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGXX)
    disable_exception_support_gcc(${targetName})
endif()

endfunction() 
