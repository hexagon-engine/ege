file(GLOB files "*" CONFIGURE_DEPENDS)
foreach(file in ${files})
    add_subdirectory(${file})
endforeach()
