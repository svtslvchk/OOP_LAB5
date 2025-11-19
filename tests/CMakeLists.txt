add_executable(tests test_pmr_vector.cpp)

# тесты должны видеть includes/
target_include_directories(tests PRIVATE
    ${PROJECT_SOURCE_DIR}/includes
)

target_link_libraries(tests PRIVATE
    lab5
    gtest
    gtest_main
)

add_test(NAME pmr_tests COMMAND tests)
