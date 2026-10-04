set(TARGET_NAME "pairs")
add_library(${TARGET_NAME} STATIC)
target_sources(${TARGET_NAME}
    PUBLIC
    FILE_SET CXX_MODULES
    FILES
    pairs.cppm
)
