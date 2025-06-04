include(FetchContent)

function(create_example TARGET_NAME SRC_FILES)
    add_executable("${TARGET_NAME}" "${SRC_FILES}")
    if (WIN32)
        set_property(GLOBAL PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    endif()
    target_link_libraries("${TARGET_NAME}" PRIVATE "${LIB_NAME}")
endfunction()

create_example("example_sf"   "examples/sf_blit.c")
create_example("example_hw"   "examples/hw_blit.c")
create_example("example_glfw" "examples/glfw_blit.c")

FetchContent_Declare(
    glfw
    GIT_REPOSITORY "https://github.com/glfw/glfw"
    GIT_TAG        "e7ea71be039836da3a98cea55ae5569cb5eb885c"
)

FetchContent_MakeAvailable(glfw)

target_include_directories("example_glfw" PRIVATE glfw)
target_link_libraries("example_glfw" PRIVATE glfw)