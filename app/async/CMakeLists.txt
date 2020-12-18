# (c) 2018 Copyright, Real-Time Innovations, Inc. All rights reserved.
# No duplications, whole or partial, manual or electronic, may be made
# without express written permission.  Any such copies, or revisions thereof,
# must display this notice unaltered.
# This code contains trade secrets of Real-Time Innovations, Inc.
cmake_minimum_required(VERSION 3.7)
project(rtiexamples-async)

find_package(test_async)
find_package(test_echo)

include(ConnextDdsAddExample)

connextdds_add_example(
    IDL "async"
    LANG C++
)

set_target_properties(app_publisher_cpp
        PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")

set_target_properties(app_subscriber_cpp
        PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")

set_target_properties(app_CXX_obj
        PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")

target_link_libraries(app_publisher_cpp
    PRIVATE 
        test::test_async
        test::test_echo
)

target_link_libraries(app_subscriber_cpp
    PRIVATE 
        test::test_async
)

