# src/password_database/CMakeLists.txt
find_package(CryptoPP REQUIRED)

set(PASSWORD_DATABASE_SOURCES
    helpers.hpp
    operations.cpp
)

add_library(password_database STATIC ${PASSWORD_DATABASE_SOURCES})

# Set include directories
target_include_directories(password_database
    PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(password_database
    PRIVATE
    CryptoPP::CryptoPP
)

target_compile_options(password_database
    PRIVATE
    /W4   # Warning level 4
    $<$<CONFIG:Debug>:/MTd>  # Use Multi-threaded Debug (/MTd) in Debug configuration
    $<$<CONFIG:Release>:/MT>  # Use Multi-threaded (/MT) in Release configuration
)
