set(CF_HEADERS ../app/neuro.h ../app/lib/qdbmp.h)
set(CF_SOURCES ../app/neuro.c ../app/lib/qdbmp.c main.c)

add_executable(${PROJECT_NAME}Training ${CF_HEADERS} ${CF_SOURCES})
target_link_libraries(${PROJECT_NAME}Training m)
