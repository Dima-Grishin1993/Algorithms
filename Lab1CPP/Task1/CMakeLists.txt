add_executable(Task1 task1.cpp)

target_include_directories(Task1 PUBLIC ../../LibraryCPP)
target_link_libraries(Task1 LibraryCPP)

add_test(NAME Test1Task1 COMMAND Task1 ${CMAKE_CURRENT_SOURCE_DIR}/Tests/input.txt)
set_property(TEST Test1Task1 PROPERTY PASS_REGULAR_EXPRESSION "")