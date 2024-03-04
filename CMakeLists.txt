cmake_minimum_required(VERSION 3.15)
project(thomas)

option(ENABLE_ASAN "Use address sanitizer" 0)
option(ENABLE_UBSAN "Use undefined behavior sanitizer" 0)

set(EXPORT_COMPILE_COMMANDS True)

set(CMAKE_C_STANDARD 20)
set(CMAKE_CXX_STANDARD 20)

if (MSVC)
    # Windows is basically garbage, so no one cares
    message(FATAL_ERROR "Windows is unsuported.")
endif()

# We are paranoid
set(UNIVERSAL_COMPILE_WARNINGS "-Wall" "-Wextra" "-Werror" "-Wcast-align" "-Wcast-qual" "-Wchar-subscripts" "-Wconversion" "-Wfloat-equal" "-Wformat-nonliteral" "-Wformat-security" "-Wformat-y2k" "-Wimport" "-Winit-self" "-Winline" "-Winvalid-pch" "-Wlong-long" "-Wmissing-braces" "-Wmissing-field-initializers" "-Wmissing-format-attribute" "-Wmissing-include-dirs" "-Wmissing-noreturn" "-Wpacked" "-Wpadded" "-Wparentheses" "-Wpointer-arith" "-Wredundant-decls" "-Wreturn-type" "-Wsequence-point" "-Wshadow" "-Wsign-compare" "-Wstack-protector" "-Wswitch" "-Wswitch-default" "-Wswitch-enum" "-Wtrigraphs" "-Wuninitialized" "-Wunknown-pragmas" "-Wunreachable-code" "-Wunused" "-Wunused-function" "-Wunused-label" "-Wunused-parameter" "-Wunused-value" "-Wunused-variable" "-Wvariadic-macros" "-Wvolatile-register-var" "-Wwrite-strings")
set(UNIVERSAL_COMPILE_SUPPRESSIONS "-Wno-padded")
# Exceptions and rtti sucks anyway
set(UNIVERSAL_COMPILE_FEATURES "-fno-exceptions" "-fno-rtti" "-g")

set(UNIVERSAL_COMPILE_OPTIONS ${UNIVERSAL_COMPILE_WARNINGS} ${UNIVERSAL_COMPILE_FEATURES} ${UNIVERSAL_COMPILE_SUPPRESSIONS})

if(APPLE)
    set(UNIVERSAL_LINKER_OPTIONS "-flto" "-fuse-ld=gold" "-g")
else()
    set(UNIVERSAL_LINKER_OPTIONS "-flto" "-g")
endif()

if(ENABLE_ASAN)
    set(UNIVERSAL_COMPILE_OPTIONS ${UNIVERSAL_COMPILE_OPTIONS} "-fsanitize=address")
    set(UNIVERSAL_LINKER_OPTIONS "-fsanitize=address")
endif()

if(ENABLE_UBSAN)
    set(UNIVERSAL_COMPILE_OPTIONS ${UNIVERSAL_COMPILE_OPTIONS} "-fsanitize=undefined")
    set(UNIVERSAL_LINKER_OPTIONS "-fsanitize=undefined")
endif()

if (CMAKE_BUILD_TYPE EQUAL "Release")
    add_compile_options(${UNIVERSAL_COMPILE_OPTIONS} "-O3")
    add_link_options(${UNIVERSAL_LINKER_OPTIONS})
else()
    add_compile_options(${UNIVERSAL_COMPILE_OPTIONS} "-O0" )
    add_link_options(${UNIVERSAL_LINKER_OPTIONS})
endif()

add_subdirectory(cmp)

add_executable(tests tests.cpp)
target_link_libraries(tests PRIVATE cmplib)

add_executable(app app.cpp)
target_link_libraries(app PRIVATE cmplib)
