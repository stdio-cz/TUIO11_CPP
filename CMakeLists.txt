cmake_minimum_required(VERSION 3.15)
project(tuio11 VERSION 1.0.0)


SET(CMAKE_CXX_STANDARD 20)
add_definitions("-Wall -Wextra -pedantic -Werror=return-type -Werror")

SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)


include_directories(oscpack)

add_library(tuio11_common STATIC
        TUIO/TuioTime.cpp
        TUIO/TuioPoint.cpp
        TUIO/TuioContainer.cpp
        TUIO/TuioObject.cpp
        TUIO/TuioCursor.cpp
        TUIO/TuioBlob.cpp
        TUIO/TuioDispatcher.cpp
        TUIO/TuioManager.cpp
        TUIO/OneEuroFilter.cpp)

add_library(tuio11_oscpack STATIC
        oscpack/osc/OscTypes.cpp
        oscpack/osc/OscOutboundPacketStream.cpp
        oscpack/osc/OscReceivedElements.cpp
        oscpack/osc/OscPrintReceivedElements.cpp
        oscpack/ip/posix/NetworkingUtils.cpp
        oscpack/ip/posix/UdpSocket.cpp)

add_library(tuio11_client STATIC
        TUIO/TuioClient.cpp
        TUIO/OscReceiver.cpp
        TUIO/UdpReceiver.cpp
        TUIO/TcpReceiver.cpp)

add_library(tuio11_server STATIC
        TUIO/TuioServer.cpp
        TUIO/UdpSender.cpp
        TUIO/TcpSender.cpp
        TUIO/WebSockSender.cpp
        TUIO/FlashSender.cpp)

target_link_libraries(tuio11_client tuio11_common tuio11_oscpack)
target_link_libraries(tuio11_server tuio11_common tuio11_oscpack)

include_directories(TUIO)
find_package(SDL2)
find_package(GLUT)
find_package(OpenGL)
if (SDL2_FOUND AND GLUT_FOUND)
    include_directories(${SDL2_INCLUDE_DIRS})

    add_executable(tuio11_simple_simulator SimpleSimulator.cpp SimpleSimulator.h)
    target_link_libraries(tuio11_simple_simulator tuio11_server ${SDL2_LIBRARIES} ${GLUT_glut_LIBRARY}  ${OPENGL_opengl_LIBRARY} ${OPENGL_glu_LIBRARY} pthread)

    add_executable(tuio11_demo TuioDemo.cpp TuioDemo.cpp)
    target_link_libraries(tuio11_demo tuio11_client ${SDL2_LIBRARIES} ${GLUT_glut_LIBRARY}  ${OPENGL_opengl_LIBRARY} ${OPENGL_glu_LIBRARY} pthread)

    add_executable(tuio11_dump TuioDump.cpp TuioDump.h)
    target_link_libraries(tuio11_dump tuio11_client pthread)

endif()