include_directories (
../thread
)
#aux_source_directory(. BASE_SRC)

set (BASE_SRC
File.cpp
Timer.cpp
Log.cpp
)
add_library (LemonBase SHARED ${BASE_SRC})

add_subdirectory (tests)
target_link_libraries (LemonBase LemonThread pthread)
