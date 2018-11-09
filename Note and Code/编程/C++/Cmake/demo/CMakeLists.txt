project(gtest)
include_directories(gtest ./)
link_directories(../lib)


add_executable(main main.cpp Add.cpp)
target_link_libraries(main libgtest.a pthread)
