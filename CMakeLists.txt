cmake_minimum_required(VERSION 3.12)
project(FinalProject)
set(CMAKE_CXX_STANDARD 11)

# Set OpenSSL dir, this should be default on linux/mac
set(OPENSSL_INCLUDE_DIR, /usr/local/opt/openssl/bin/openssl)
#set(CMAKE_PREFIX_PATH, /usr/local/opt/nlohmann_json/include/nlohmann/)

# Get Boost System
find_package(Boost REQUIRED system thread chrono)

# Get OpenSSL
find_package(openssl REQUIRED)

# Get cppRestSDK
find_package(cpprestsdk REQUIRED)


# Compile + Link it all together
add_executable(FinalProject main.cpp User.cpp User.h)
target_link_libraries(FinalProject cpprestsdk::cpprest ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_CHRONO_LIBRARY})