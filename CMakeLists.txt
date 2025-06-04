cmake_minimum_required(VERSION 3.28.3)

project(pix VERSION 1.0.0 LANGUAGES C CXX)

set(LIB_NAME "pix")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SRC_FILES src/*.c)

add_library(${LIB_NAME} 
    "src/libgl/pix_gl.c"
    "src/libgl/pix_gl_win32.c"
    "src/libwin/pix_window.c"
    "src/libwin/win32/pix_bitmap_win32.c"
    "src/libwin/win32/pix_context_win32.c"
    "src/libwin/win32/pix_thunk.c"
    "src/libwin/win32/pix_window_win32.c"
    "src/libwin/x11/pix_window_x11.c"
    "src/libthread/pix_thread_win32.c"
    "src/libtime/pix_time_win32.c"
)

target_compile_features(${LIB_NAME} PRIVATE c_std_99)
target_include_directories(${LIB_NAME} PUBLIC include)

target_compile_options(${LIB_NAME} PRIVATE $ENV{LIB_COMPILE_OPTIONS})

find_library(C_MATH_LIBRARY m)
if(C_MATH_LIBRARY)
    target_link_libraries(${LIB_NAME} PUBLIC ${C_MATH_LIBRARY})
endif()

if (LINUX)
    target_compile_definitions(${LIB_NAME} PUBLIC PIX_PLATFORM_LINUX)
    find_package(X11 REQUIRED COMPONENTS xcb_image)
    find_package(Freetype REQUIRED)
    find_package(Fontconfig REQUIRED)
    target_include_directories(${LIB_NAME} PRIVATE 
        ${X11_xcb_INCLUDE_PATH} 
        ${X11_xcb_image_INCLUDE_PATH} 
        ${FREETYPE_INCLUDE_DIRS} 
        ${Fontconfig_INCLUDE_DIRS}
    )
    target_link_libraries(${LIB_NAME} 
        ${X11_xcb_LIB} 
        ${X11_xcb_image_LIB} 
        ${FREETYPE_LIBRARIES}
        ${Fontconfig_LIBRARIES}
    )
    target_compile_options(${LIB_NAME} PRIVATE ${Fontconfig_COMPILE_OPTIONS})
elseif(WIN32)
    target_compile_definitions(${LIB_NAME} PUBLIC PIX_PLATFORM_WIN32)
    # statically link CRT
    set_property(TARGET "${LIB_NAME}" PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
else()
    message(FATAL_ERROR "unsupported system")
endif()

include(examples/CMakeLists.txt)

function(folder_exists PATH_NAME)
    get_filename_component(RESOLVED_PATH "${PATH_NAME}" REALPATH)
    if (EXISTS "${RESOLVED_PATH}" AND IS_DIRECTORY "${RESOLVED_PATH}")
        set(RETVAR True PARENT_SCOPE)
    else()
        set(RETVAR False PARENT_SCOPE)
    endif()
endfunction()

set(BLEND2D_PATH "${CMAKE_CURRENT_LIST_DIR}/vendor/blend2d")
folder_exists("${BLEND2D_PATH}")

if (${RETVAR})
    add_executable(blend2d_test test/blend2d.c)
    set(BLEND2D_STATIC TRUE)
    include("${BLEND2D_PATH}/CMakeLists.txt")
    target_link_libraries(blend2d_test blend2d::blend2d)
    target_link_libraries(blend2d_test "${LIB_NAME}")
else()
    message(WARNING "blend2d not present at ${BLEND2D_PATH}: not building blend2d test")
endif()

set(SKIA_PATH "${CMAKE_CURRENT_LIST_DIR}/vendor/skia")
folder_exists("${SKIA_PATH}")

if (${RETVAR})
    set(SKIA_LIB_PATH "${SKIA_PATH}/out/Shared/libskia.so")
    if (EXISTS "${SKIA_LIB_PATH}" AND NOT IS_DIRECTORY "${SKIA_LIB_PATH}")
        add_executable(skia_test test/skia.cpp)
        target_include_directories(skia_test PRIVATE "${SKIA_PATH}")
        target_link_libraries(skia_test "${SKIA_LIB_PATH}")
        target_link_libraries(skia_test "${LIB_NAME}")
    else()
        message(WARNING "Expected libskia.so @ ${SKIA_LIB_PATH}. Build Skia to resolve this warning.")
    endif()
else()
    message(WARNING "Skia not present at ${SKIA_PATH}: not building Skia test")
endif()