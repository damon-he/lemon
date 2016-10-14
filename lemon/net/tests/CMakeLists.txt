include_directories (
..
../../boost_1_61_0)
link_directories (..)
set (UDP_SOCKET_TEST_SRC
UdpSocketTest.cpp)
add_executable (UdpSocketTest ${UDP_SOCKET_TEST_SRC})
target_link_libraries (UdpSocketTest LemonNet pthread)

include_directories (
..
../../thread
)
link_directories (
..
../../thread
)
set (TCP_SOCKET_TEST_SRC
TcpSocketTest.cpp)
add_executable (TcpSocketTest ${TCP_SOCKET_TEST_SRC})
target_link_libraries (TcpSocketTest LemonNet LemonThread pthread)
