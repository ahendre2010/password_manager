# src/password_manager/CMakeLists.txt

# Add source files for password_manager
set(SOURCES
    main.cpp
)

# Add executable target for password_manager
add_executable(password_manager ${SOURCES})

# Set include directories if needed
target_include_directories(password_manager
    PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# Set output directory structure
set_target_properties(password_manager PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/exe/Debug"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/exe/Release"
)

# Set additional compiler options
target_compile_options(password_manager
    PRIVATE
    /W4   # Warning level 4
    $<$<CONFIG:Debug>:/MTd>  # Use Multi-threaded Debug (/MTd) in Debug configuration
    $<$<CONFIG:Release>:/MT>  # Use Multi-threaded (/MT) in Release configuration
)
