set(CF_HEADERS ./lib/qdbmp.h neuro.h)
set(CF_SOURCES ./lib/qdbmp.c neuro.c main.c)

add_executable(${PROJECT_NAME} ${CF_HEADERS} ${CF_SOURCES})
target_link_libraries(${PROJECT_NAME} m)
