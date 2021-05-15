enable_testing()
if (UNIX)
    find_package(Boost COMPONENTS unit_test_framework REQUIRED)
endif ()
file(GLOB test_SRC "*.cpp")

add_executable(tin_test ${test_SRC})
target_link_libraries(tin_test ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} tin_lib pthread)

add_test(AllTests tin_test)

