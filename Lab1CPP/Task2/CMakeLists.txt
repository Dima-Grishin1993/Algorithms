add_executable(Task2 task2.cpp)

target_include_directories(Task2 PUBLIC ../../LibraryCPP)
target_link_libraries(Task2 LibraryCPP)

add_test(NAME Test2Task2 COMMAND Task2 ${CMAKE_CURRENT_SOURCE_DIR}/Tests/input.txt)
set_property(TEST Test2Task2 PROPERTY PASS_REGULAR_EXPRESSION "2.*4")

add_test(NAME Task2_Test2 COMMAND Task2 ${CMAKE_CURRENT_SOURCE_DIR}/Tests/input2.txt)
set_property(TEST Task2_Test2 PROPERTY PASS_REGULAR_EXPRESSION "0.*1000")

add_test(NAME Task2_Test3 COMMAND Task2 ${CMAKE_CURRENT_SOURCE_DIR}/Tests/input3.txt)
set_property(TEST Task2_Test3 PROPERTY PASS_REGULAR_EXPRESSION "9")