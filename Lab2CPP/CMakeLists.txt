add_executable(Lab2CPP lab2.cpp)
target_include_directories(Lab2CPP PUBLIC ../LibraryCPP)
target_link_libraries(Lab2CPP LibraryCPP)

# Запускает программу на входном файле и проверяет вывод регулярным выражением
function(add_lab2_test name file regex)
    add_test(NAME ${name} COMMAND Lab2CPP ${CMAKE_CURRENT_SOURCE_DIR}/Tests/${file})
    set_tests_properties(${name} PROPERTIES PASS_REGULAR_EXPRESSION "${regex}")
endfunction()

# Пример из задания
add_lab2_test(TestLab2CPP input.txt
    "stack:\n1\nvars:\n5\n0\n0\n0\n")

# Все арифметические операции, swap, pop, iload/istore
add_lab2_test(TestLab2CPPArith test_arith.txt
    "stack:\n-18\n7\nvars:\n0\n6\n0\n0\n")

# Вызов функций через invokestatic/return
add_lab2_test(TestLab2CPPCalls test_calls.txt
    "call 10\ncall 1\nstack:\n1\nvars:\n7\n6\n0\n42\n")

# Ошибки
add_lab2_test(TestLab2CPPPopAddress err_pop_address.txt
    "error: pop of return address")
add_lab2_test(TestLab2CPPStoreAddress err_store_address.txt
    "error: istore_0 of return address")
add_lab2_test(TestLab2CPPArithAddress err_arith_address.txt
    "error: iadd with return address")
add_lab2_test(TestLab2CPPReturnNumber err_return_number.txt
    "error: return with a number")
add_lab2_test(TestLab2CPPEmpty err_empty.txt
    "error: not enough operands for iadd")
add_lab2_test(TestLab2CPPUnknown err_unknown.txt
    "error: unknown command")