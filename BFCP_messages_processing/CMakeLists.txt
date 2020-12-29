project(bfcp_message)
cmake_minimum_required(VERSION 2.8)

set(SRCS bfcp_messages.c
  bfcp_messages_build.c
  bfcp_messages_parse.c
  )

set(HEADERS bfcp_strings.h bfcp_messages.h)

include_directories(${CMAKE_SOURCE_DIR})
link_directories(${CMAKE_SOURCE_DIR})

if(WIN32)
else()
  set(CMAKE_CFLAGS "-Wall -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -O2 -Werror -Wunused")
endif()

# 生成动态库目标
add_library(${PROJECT_NAME} SHARED ${HEADERS} ${SRCS})
# 生成静态库目标
add_library(${PROJECT_NAME}_static STATIC ${HEADERS} ${SRCS})

add_executable(${PROJECT_NAME}_test "testcode.c")

if(WIN32)
  target_link_libraries(${PROJECT_NAME} ws2_32)
  target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME}_static ws2_32)
else()
  target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME}_static pthread)
endif()

# 指定静态库的输出名称
set_target_properties(${PROJECT_NAME}_static PROPERTIES OUTPUT_NAME "bfcp_static")
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "bfcp")
# 使动态库和静态库同时存在
set_target_properties(${PROJECT_NAME}_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
set_target_properties(${PROJECT_NAME} PROPERTIES CLEAN_DIRECT_OUTPUT 1)

