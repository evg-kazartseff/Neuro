set(CF_HEADERS neuro.h)
set(CF_SOURCES neuro.c main.c)

add_executable(${PROJECT_NAME} ${CF_HEADERS} ${CF_SOURCES})
target_link_libraries(${PROJECT_NAME} m)
