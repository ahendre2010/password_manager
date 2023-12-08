set(3RD_PARTY_DIR "${CMAKE_SOURCE_DIR}/3rd_party")
set(CRYPTOPP_ROOT "${3RD_PARTY_DIR}/cryptopp")

add_library(CryptoPP::CryptoPP STATIC IMPORTED)

target_include_directories(CryptoPP::CryptoPP
INTERFACE
  "${CRYPTOPP_ROOT}/include")

set_property(TARGET CryptoPP::CryptoPP APPEND PROPERTY
IMPORTED_CONFIGURATIONS
 RELEASE)

set_property(TARGET CryptoPP::CryptoPP APPEND PROPERTY
IMPORTED_CONFIGURATIONS
 DEBUG)

set_target_properties(CryptoPP::CryptoPP PROPERTIES
IMPORTED_LOCATION_RELEASE
    "${CRYPTOPP_ROOT}/lib/x64/release/cryptlib.lib")

set_target_properties(CryptoPP::CryptoPP PROPERTIES
IMPORTED_LOCATION_DEBUG
    "${CRYPTOPP_ROOT}/lib/x64/debug/cryptlib.lib")

set(CRYPTOPP_FOUND 1)
