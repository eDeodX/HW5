cmake_minimum_required(VERSION 3.10)
project(account)
project(transaction)

add_library(account STATIC ${CMAKE_CURRENT_SOURCE_DIR}/Account.cpp)
add_library(transaction STATIC ${CMAKE_CURRENT_SOURCE_DIR}/Transaction.cpp)