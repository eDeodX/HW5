cmake_minimum_required(VERSION 3.10)
project(test_account)
project(test_transactiom)

add_executable(test_account ${CMAKE_CURRENT_SOURCE_DIR}/test_account.cpp)
target_link_libraries(test_account PUBLIC gtest gtest_main account)

add_executable(test_transaction ${CMAKE_CURRENT_SOURCE_DIR}/test_transaction.cpp)
target_link_libraries(test_transaction PUBLIC gtest gtest_main gmock gmock_main transaction account)