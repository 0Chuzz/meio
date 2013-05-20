
cmake_minimum_required(VERSION 2.8)
project(MEIO)

include_directories("include")
find_program(re2c_exe "re2c")
aux_source_directory("src" SOURCES)
set(lexer_unparsed "src/lexer.c")
string(REPLACE "src/lexer.c" "lexer.re2c.c" lexer_parsed ${lexer_unparsed})
set(lexer_parsed "${CMAKE_CURRENT_BINARY_DIR}/${lexer_parsed}")
add_custom_command(
    OUTPUT ${lexer_parsed}
    COMMAND ${re2c_exe} -g -o ${lexer_parsed}
        "${CMAKE_CURRENT_SOURCE_DIR}/${lexer_unparsed}"
    DEPENDS ${lexer_unparsed})
message(${SOURCES})
list(REMOVE_ITEM SOURCES ${lexer_unparsed})
list(APPEND SOURCES ${lexer_parsed})
set_source_files_properties(${lexer_parsed} PROPERTIES GENERATED 1)
message(${SOURCES})
add_executable(meio ${SOURCES})

