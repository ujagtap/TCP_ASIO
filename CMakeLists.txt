cmake_minimum_required(VERSION 2.8.4)
project(TCP_ASIO)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
SET(Boost_USE_STATIC_LIBS OFF)
SET(Boost_USE_MULTITHREAD ON)
set(BOOST_INCLUDEDIR /opt/twitter/include)
set(BOOST_LIBRARYDIR /opt/twitter/lib)

FIND_PACKAGE(Boost 1.42.0 REQUIRED COMPONENTS system thread regex)

IF (Boost_FOUND)
  INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
  LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
ENDIF(Boost_FOUND)

SET(USED_LIBS ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_REGEX_LIBRARY})

ADD_EXECUTABLE(server main.cpp udp_server.cpp connection.cpp server.cpp)
ADD_EXECUTABLE(client client.cpp)
ADD_EXECUTABLE(udp_client udp_client.cpp)

TARGET_LINK_LIBRARIES(server ${USED_LIBS})
TARGET_LINK_LIBRARIES(client ${USED_LIBS})
TARGET_LINK_LIBRARIES(udp_client ${USED_LIBS})