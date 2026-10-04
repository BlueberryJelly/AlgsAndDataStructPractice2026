set(PRACTICE_NAME practice_2)

add_executable(${PRACTICE_NAME} main.cpp)
target_link_libraries(${PRACTICE_NAME}
    PRIVATE
    pairs
)