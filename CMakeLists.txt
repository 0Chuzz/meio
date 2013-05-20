
project(MEIO)

include_directories("include/")
aux_source_directory("src/" SOURCES)
add_executable(meio ${SOURCES})

