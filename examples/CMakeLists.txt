macro(ege_add_example name sources ege_deps)
    set(sources_with_dir "")
    foreach(source ${sources})
        list(APPEND sources_with_dir ${name}/${source})
    endforeach()

    set(ege_deps_with_ege "")
    foreach(dep ${ege_deps})
        list(APPEND ege_deps_with_ege ege-${dep})
    endforeach()

    add_executable(${name} ${sources_with_dir})
    target_link_libraries(${name} ${ege_deps_with_ege})
    target_include_directories(${name} PRIVATE ${CMAKE_SOURCE_DIR})
    install(TARGETS ${name} RUNTIME DESTINATION examples/${name})
    
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${name}/res)
        install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${name}/res DESTINATION ${name})
    endif()
endmacro()

ege_add_example(complexLoader           "main.cpp"  scene)
ege_add_example(physicalParticleSystem  "main.cpp"  scene)
