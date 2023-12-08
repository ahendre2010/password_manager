# src/password_database/CMakeLists.txt

# Add source files for the password_database library
set(PASSWORD_DATABASE_SOURCES
    helpers.hpp
    operations.cpp
)

# Add library target for password_database
add_library(password_database STATIC ${PASSWORD_DATABASE_SOURCES})

# Set include directories if needed
target_include_directories(password_database
    PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# Set additional compiler options
target_compile_options(password_database
    PRIVATE
    /W4   # Warning level 4
    $<$<CONFIG:Debug>:/MTd>  # Use Multi-threaded Debug (/MTd) in Debug configuration
    $<$<CONFIG:Release>:/MT>  # Use Multi-threaded (/MT) in Release configuration
)
