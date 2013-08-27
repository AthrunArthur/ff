project(ff)

include_directories(/home/athrun/boost_1_54_0/ /root/projects/ff/)
link_directories(/home/athrun/boost_1_54_0/stage/lib/)

add_executable(ff main.cpp)
target_link_libraries(ff boost_thread boost_system boost_date_time)


install(TARGETS ff RUNTIME DESTINATION bin)
