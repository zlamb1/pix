cmake_minimum_required(VERSION 3.28.3)

project(pix VERSION 1.0.0 LANGUAGES C CXX)

set(LIB_NAME "pix")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SRC_FILES src/*.c)

# library

add_library(${LIB_NAME} ${SRC_FILES})
target_compile_features(${LIB_NAME} PRIVATE c_std_99)
target_include_directories(${LIB_NAME} PUBLIC include)
target_compile_options(${LIB_NAME} PRIVATE 
    -Wall -Wextra -Wpedantic -Werror -Wshadow -Wpointer-arith -Wstrict-prototypes -Wmissing-prototypes
    -Wno-missing-braces -Wno-missing-field-initializers -Wbad-function-cast -Winline -Wundef
    -Wunreachable-code -Wlogical-op -Wredundant-decls -Wfloat-equal -Wcast-align -Wcast-qual
    -Wdeclaration-after-statement -Wmissing-include-dirs -Wnested-externs -Wsequence-point
    -Wswitch -Wunused-but-set-parameter -Wwrite-strings -Wunused-result -pedantic-errors 
    --param max-inline-insns-single=1000 -O2
)

target_compile_options(${LIB_NAME} PUBLIC -O2 -msse2 -mavx)

target_link_libraries(${LIB_NAME} m)

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
else()
    message(FATAL_ERROR "unsupported system")
endif()

# main executable

add_executable(${LIB_NAME}_exe src/main.c)
set_property(TARGET ${LIB_NAME}_exe PROPERTY OUTPUT_NAME ${PROJECT_NAME})
target_link_libraries(${LIB_NAME}_exe ${PROJECT_NAME})

# testing

add_executable(blend2d_test test/blend2d.c)
set(BLEND2D_STATIC TRUE)
include("${CMAKE_CURRENT_LIST_DIR}/vendor/blend2d/CMakeLists.txt")
target_link_libraries(blend2d_test blend2d::blend2d)
target_link_libraries(blend2d_test ${LIB_NAME})

add_executable(skia_test test/skia.cpp)
target_include_directories(skia_test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/vendor/skia)
target_link_libraries(skia_test ${CMAKE_CURRENT_LIST_DIR}/vendor/skia/out/Shared/libskia.so)
target_link_libraries(skia_test ${LIB_NAME})