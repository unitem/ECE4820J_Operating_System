project(lemondb)
cmake_minimum_required(VERSION 2.7)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS
    "-Wall -Wextra -Werror -pedantic -Wno-unused-result -Wconversion -Wno-unused-parameter -O2"
)
set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=-*,clang-analyzer-*,performance-*,misc-*;-fix")
set(CMAKE_CXX_CPPCHECK "cppcheck;--quiet")

set(SOURCE_FILES
    db/Database.h
    db/Database.cpp
    db/Table.h
    db/Table.cpp
    query/Query.h
    query/Query.cpp
    query/QueryParser.h
    query/QueryParser.cpp
    query/QueryResult.h
    query/QueryResult.cpp
    query/QueryBuilders.h
    query/QueryBuilders.cpp
    query/management/CopyTableQuery.h
    query/management/CopyTableQuery.cpp
    query/management/DropTableQuery.h
    query/management/DropTableQuery.cpp
    query/management/DumpTableQuery.h
    query/management/DumpTableQuery.cpp
    query/management/ListTableQuery.h
    query/management/ListTableQuery.cpp
    query/management/LoadTableQuery.h
    query/management/LoadTableQuery.cpp
    query/management/PrintTableQuery.h
    query/management/PrintTableQuery.cpp
    query/management/QuitQuery.h
    query/management/QuitQuery.cpp
    query/management/TruncateTableQuery.h
    query/management/TruncateTableQuery.cpp
    query/data/DeleteQuery.h
    query/data/DeleteQuery.cpp
    query/data/DuplicateQuery.h
    query/data/DuplicateQuery.cpp
    query/data/InsertQuery.h
    query/data/InsertQuery.cpp
    query/data/MaxQuery.h
    query/data/MaxQuery.cpp
    query/data/SelectQuery.h
    query/data/SelectQuery.cpp
    query/data/SubQuery.h
    query/data/SubQuery.cpp
    query/data/SumQuery.h
    query/data/SumQuery.cpp
    query/data/SwapQuery.h
    query/data/SwapQuery.cpp
    query/data/UpdateQuery.h
    query/data/UpdateQuery.cpp
    utils/formatter.h
    utils/uexception.h)
add_executable(lemondb main.cpp ${SOURCE_FILES})
target_link_libraries(lemondb pthread)
