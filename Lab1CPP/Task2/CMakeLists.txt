add_executable(Task2 task2.cpp)

target_include_directories(Task2 PUBLIC ../../LibraryCPP)
target_link_libraries(Task2 LibraryCPP)

add_test(NAME Test1Task2 COMMAND Task2 ${CMAKE_CURRENT_SOURCE_DIR}/Tests/input.txt)
set_property(TEST Test1Task2 PROPERTY PASS_REGULAR_EXPRESSION "")