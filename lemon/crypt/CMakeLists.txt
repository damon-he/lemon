include_directories (
../mavlink/common
../base 
.)

set (CRYPT_SRC
AES128Crypt.cpp
aes.c
)
add_library (LemonCrypt SHARED ${CRYPT_SRC})

add_subdirectory (tests)
