project(ff)
add_definitions(-std=c++11)
add_definitions(-g)
include_directories(../)
add_executable(ff main.cpp)

install(TARGETS ff RUNTIME DESTINATION bin)
target_link_libraries(ff pthread)
