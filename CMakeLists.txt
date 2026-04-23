cmake_minimum_required(VERSION 3.10)
project(banking)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	add_compile_options(--coverage)
	add_link_options(--coverage)
endif()

if(MSVC)
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
endif()

add_library(account STATIC ${CMAKE_CURRENT_SOURCE_DIR}/banking/Account.cpp)
add_library(transaction STATIC ${CMAKE_CURRENT_SOURCE_DIR}/banking/Transaction.cpp)

target_link_libraries(transaction PUBLIC account)
target_include_directories(account PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/banking)
target_include_directories(transaction PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/banking)

include(FetchContent)
FetchContent_Declare(
	googletest
	GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(test_account ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_account.cpp)
target_link_libraries(test_account PUBLIC gtest gtest_main gmock gmock_main account)

add_executable(test_transaction ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_transaction.cpp)
target_link_libraries(test_transaction PUBLIC gtest gtest_main gmock gmock_main transaction account)

add_test(NAME AccountTest COMMAND test_account)
add_test(NAME TransactionTest COMMAND test_transaction)