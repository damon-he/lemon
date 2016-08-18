include_directories (../base .)
set (THREAD_SRC
Thread.cpp
ThreadPool.cpp)
add_library (LemonThread SHARED ${THREAD_SRC})

add_subdirectory (tests)
