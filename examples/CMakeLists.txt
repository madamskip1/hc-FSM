set(examples "")


foreach(example ${examples})
    add_executable(
        ${example}
        ${example}/main.cpp
    )
    target_link_libraries(
        ${example} 
        PRIVATE 
        hcFSM::hcFSM
    )
endforeach()
