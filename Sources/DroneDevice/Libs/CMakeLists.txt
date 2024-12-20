file(GLOB SOURCE_FILES "libcanard/*.c")
add_library(libcanard_sources OBJECT ${SOURCE_FILES})
target_compile_options(libcanard_sources PRIVATE SHELL:${FLAGS_CPU} SHELL:${FLAGS_COMMON})
