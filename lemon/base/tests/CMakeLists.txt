include_directories (..)
link_directories (..)
set (FILE_TEST_SRC
FileTest.cpp)
add_executable (file_test ${FILE_TEST_SRC})
target_link_libraries (file_test LemonBase)

set (TIMER_TEST_SRC
TimerTest.cpp)
add_executable (TimerTest ${TIMER_TEST_SRC})
target_link_libraries (TimerTest LemonBase)

set (Log_TEST_SRC
LogTest.cpp)
add_executable (LogTest ${Log_TEST_SRC})
target_link_libraries (LogTest LemonBase)
