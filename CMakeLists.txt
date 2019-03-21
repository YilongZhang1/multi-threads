cmake_minimum_required (VERSION 3.6)
project(multi_threads)

set(CMAKE_C_FLAGS "-std=c11 -Wall -Wextra -Wshadow -Werror")
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Wshadow -Werror")

include_directories (${PROJECT_SOURCE_DIR}/include)

#add_library (csvFileParser SHARED src/csvFileParser.cpp)
#install (TARGETS csvFileParser DESTINATION lib)
#INSTALL(FILES include/fileParsing.h DESTINATION include)

add_executable(${PROJECT_NAME} src/main.cpp src/csvFileParser.cpp src/argMax_and_distanceMatrix.cpp src/SegThreader.cpp src/DistMatrixThreader.cpp src/AverageThreader.cpp)

#target_link_libraries (${PROJECT_NAME} csvFileParser argMax_and_distanceMatrix)
target_link_libraries(${PROJECT_NAME} boost_thread-mt boost_system)