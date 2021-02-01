cmake_minimum_required(VERSION 3.16)
set (CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_BUILD_TYPE "Release")

project(ladybug VERSION 1.0.0 LANGUAGES CXX)

add_subdirectory(lib)

if(
    TARGET sioclient AND
    TARGET nlohmann_json
)
    message("all libraries found")

    add_executable(ladybug
        main.cpp
        hub.cpp
        hub.hpp
    )

    target_link_libraries(ladybug
        sioclient
        nlohmann_json::nlohmann_json
    )



#  if (MSVC)
#    set_property(SOURCE main.cpp APPEND APPEND_STRING PROPERTY COMPILE_FLAGS "/wd4251 /wd4127 /wd4275")
#  endif()


#list(APPEND
#     LICENSES
#     "../licenses/Copyright.txt"
#     "../licenses/Boost_LICENSE.txt"
#)
#file(COPY ${LICENSES} DESTINATION "Release/licenses/")
#INSTALL(FILES ${LICENSES} DESTINATION "Release/licenses/")

else()
    message(FATAL_ERROR "Didn't find all libraries")
endif()
