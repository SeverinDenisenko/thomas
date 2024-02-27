cmake_minimum_required(VERSION 3.15)
project(thomas)

set(EXPORT_COMPILE_COMMANDS True)

set(CMAKE_C_STANDARD 20)
set(CMAKE_CXX_STANDARD 20)

if (MSVC)
    # Windows is basically garbage, so no one cares
    set(UNIVERSAL_COMPILE_WARNINGS )
    set(UNIVERSAL_COMPILE_SUPPRESSIONS )
    set(UNIVERSAL_COMPILE_FEATURES )
else()
    # We are paranoid
    set(UNIVERSAL_COMPILE_WARNINGS "-Wall" "-Wextra" "-Werror" "-Wcast-align" "-Wcast-qual" "-Wchar-subscripts" "-Wconversion" "-Wfloat-equal" "-Wformat-nonliteral" "-Wformat-security" "-Wformat-y2k" "-Wimport" "-Winit-self" "-Winline" "-Winvalid-pch" "-Wlong-long" "-Wmissing-braces" "-Wmissing-field-initializers" "-Wmissing-format-attribute" "-Wmissing-include-dirs" "-Wmissing-noreturn" "-Wpacked" "-Wpadded" "-Wparentheses" "-Wpointer-arith" "-Wredundant-decls" "-Wreturn-type" "-Wsequence-point" "-Wshadow" "-Wsign-compare" "-Wstack-protector" "-Wswitch" "-Wswitch-default" "-Wswitch-enum" "-Wtrigraphs" "-Wuninitialized" "-Wunknown-pragmas" "-Wunreachable-code" "-Wunused" "-Wunused-function" "-Wunused-label" "-Wunused-parameter" "-Wunused-value" "-Wunused-variable" "-Wvariadic-macros" "-Wvolatile-register-var" "-Wwrite-strings")
    set(UNIVERSAL_COMPILE_SUPPRESSIONS "-Wno-padded")
    # Exceptions and rtti sucks anyway
    set(UNIVERSAL_COMPILE_FEATURES "-fno-exceptions" "-fno-rtti")
endif()

set(UNIVERSAL_COMPILE_OPTIONS ${UNIVERSAL_COMPILE_WARNINGS} ${UNIVERSAL_COMPILE_FEATURES} ${UNIVERSAL_COMPILE_SUPPRESSIONS})

if (CMAKE_BUILD_TYPE EQUAL "Release")
    add_compile_options(${UNIVERSAL_COMPILE_OPTIONS} "-O3" "-flto")
    add_link_options("-flto")
else()
    add_compile_options(${UNIVERSAL_COMPILE_OPTIONS} "-O0" "-fsanitize=address")
    add_link_options("-fsanitize=address")
endif()

add_subdirectory(cmp)

add_executable(tests tests.cpp)
target_link_libraries(tests PRIVATE cmplib)

add_executable(app app.cpp)
target_link_libraries(app PRIVATE cmplib)
