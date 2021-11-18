cmake_minimum_required(VERSION 2.8)

if(CMAKE_BUILD_TYPE MATCHES Release)
    message("Release Build")
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin/release)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/release)
else(CMAKE_BUILD_TYPE MATCHES Release)
    message("Debug Build")
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin/debug)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/debug)
endif(CMAKE_BUILD_TYPE MATCHES Release)

if(UNIX)
    add_definitions(-DLINUX=1)
    if(PROFILING)
        message("Option de profilage ajouté!")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")
    endif(PROFILING)
endif(UNIX)


add_definitions(-std=c++11)

# une image PNG par frame: utiliser -DPNGFILES=ON
if(PNGFILES)
    add_definitions(-DFIMAGE)
endif(PNGFILES)

project(Projet CXX C)
add_executable(server src/server.cpp src/CameraCapture.cpp)
add_executable(client src/client.cpp)

if(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif(MSVC)

target_include_directories(server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(client PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include /usr/include/)
find_package(Threads)
target_link_libraries(server ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(client tesseract lept)
target_link_libraries(client ${CMAKE_THREAD_LIBS_INIT})

find_package(OpenCV REQUIRED)
if(OpenCV_FOUND)
    include_directories(${OpenCV_INCLUDE_DIRS})
    target_link_libraries(server ${OpenCV_LIBRARIES})
    target_link_libraries(client ${OpenCV_LIBRARIES})
else(OpenCV_FOUND)
    message(FATAL ERROR "Librarie OpenCV introuvable!")
endif(OpenCV_FOUND)
