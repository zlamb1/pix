cmake_minimum_required(VERSION 3.28.3)

project(phoenix VERSION 1.0.0 LANGUAGES CXX)

file(GLOB_RECURSE SRC_FILES src/*.cpp)

add_executable(phoenix ${SRC_FILES})
target_include_directories(${PROJECT_NAME} include)

add_compile_definitions(PX_PLATFORM=${CMAKE_SYSTEM_NAME})

if (LINUX)
    # X11 / XCB
    find_package(X11 REQUIRED)

    if(NOT ${X11_xcb_FOUND})
        message(FATAL_ERROR "missing xcb")
    endif()

    target_include_directories(${PROJECT_NAME} PRIVATE ${X11_xcb_INCLUDE_PATH})
    target_link_libraries(${PROJECT_NAME} ${X11_xcb_LIB})
else()
    message(FATAL_ERROR "unsupported system")
endif()