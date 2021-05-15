#define BOOST_TEST_MODULE "TIN_boost_test"
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <vector>
#include "../include/SessionController.h"




BOOST_AUTO_TEST_SUITE(NodesTests)
    BOOST_AUTO_TEST_CASE(test_NONE_to_NONE) {
        BOOST_CHECK_EQUAL(2 + 3, 4); //not passed
    }
    BOOST_AUTO_TEST_CASE(test_LEADER_to_NONE) {
        BOOST_CHECK_EQUAL(2 * 2, 4); //passed
    }
BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(mocktest)
    BOOST_AUTO_TEST_CASE(test1) {
        BOOST_CHECK_EQUAL(1, 1); // passed
    }
BOOST_AUTO_TEST_SUITE_END()