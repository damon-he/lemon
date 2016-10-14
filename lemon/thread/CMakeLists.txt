include_directories (
../base 
./
../boost_1_61_0
)
set (THREAD_SRC
Thread.cpp
ThreadPool.cpp
)
add_library (LemonThread SHARED ${THREAD_SRC})

add_subdirectory (tests)
