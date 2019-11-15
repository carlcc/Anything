
add_library(anything STATIC 
    anything/AnythingConfig.h
    anything/AnythingConfig.cpp
    anything/FileEnumerator.h
    anything/FileEnumerator.cpp
    anything/Anything.h
    anything/Anything.cpp
    anything/StringUtil.h
    anything/ATVector.h
)
target_link_libraries(anything
    cppfs
    jsoncpp
)
target_include_directories(anything PRIVATE
    .
)


add_executable(anything_exe 
    cli/main.cpp
)
target_link_libraries(anything_exe
    anything
    cppfs
    jsoncpp
    fmt
    pcre
)
target_include_directories(anything_exe PRIVATE
    .
)
add_dependencies(anything_exe anything)


if (AT_ENABLE_QT_GUI)
    add_subdirectory(qtgui/anythinggui)
endif()

# tests
add_executable(test_atvector tests/test_atvector.cpp)
target_link_libraries(test_atvector
    anything
)
target_include_directories(test_atvector PRIVATE
    .
)
add_dependencies(test_atvector anything)


add_executable(test_log tests/test_log.cpp)
target_include_directories(test_log PRIVATE
    .
)
target_link_libraries(test_log
    fmt
)

add_executable(test_cppfs tests/test_cppfs.cpp)
target_include_directories(test_cppfs PRIVATE
    .
)
target_link_libraries(test_cppfs
    fmt cppfs
)
