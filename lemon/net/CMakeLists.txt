include_directories (
../mavlink/common
../base 
.)

set (NET_SRC
Socket.cpp
UdpSocket.cpp
TcpSocket.cpp
)
add_library (LemonNet SHARED ${NET_SRC})

add_subdirectory (tests)
