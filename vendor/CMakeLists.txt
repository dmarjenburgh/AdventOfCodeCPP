add_library(hashlib2plus STATIC
        hashlib2plus/hl_md5.cpp
        hashlib2plus/hl_sha1wrapper.cpp
        hashlib2plus/hl_sha2ext.cpp
        hashlib2plus/hl_sha384wrapper.cpp
        hashlib2plus/hl_sha1.cpp
        hashlib2plus/hl_sha256wrapper.cpp
        hashlib2plus/hl_md5wrapper.cpp
        hashlib2plus/hl_sha256.cpp
        hashlib2plus/hl_sha512wrapper.cpp
        hashlib2plus/hl_wrapperfactory.cpp)

target_include_directories(hashlib2plus PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/hashlib2plus)
