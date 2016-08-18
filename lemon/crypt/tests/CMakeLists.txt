include_directories (
..
../../net
)

link_directories (
..
../../net
)

set (AES128_CRYPT_TEST_SRC
AES128CryptTest.cpp
)
add_executable (AES128CryptTest ${AES128_CRYPT_TEST_SRC})
target_link_libraries (AES128CryptTest pthread LemonNet LemonCrypt)

