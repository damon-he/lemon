include_directories (.. 
../../base
../../boost_1_61_0)
link_directories (..)
set (THREAD_TEST_SRC
ThreadTest.cpp)
add_executable (ThreadTest ${THREAD_TEST_SRC})
target_link_libraries (ThreadTest LemonThread pthread)
