cmake_minimum_required(VERSION 3.28)
project(DofeProject)

set(CMAKE_CXX_STANDARD 17)

# Find libcurl
find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIR})

# include sqlite3
find_package(SQLite3)
include_directories(${SQLite3_INCLUDE_DIRS})

# Fetch and include json library
include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

add_executable(DofeProject
        main.cpp
        library/library.cpp
        database/database.h
        database/database.cpp
        utils/utils.cpp
        utils/utils.h
)

# Link libraries using keyword signature
target_link_libraries(DofeProject PRIVATE ${CURL_LIBRARIES} ${SQLite3_LIBRARIES} nlohmann_json::nlohmann_json)
