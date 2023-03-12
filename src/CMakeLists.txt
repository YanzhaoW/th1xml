project(support)

set(HEADERS Options.hpp)

set(SRCS)

# add_library(${PROJECT_NAME} ${HEADERS} ${SRCS})
add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME}
                           INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} INTERFACE Boost::program_options
                                                ROOT::RIO ROOT::Hist)
