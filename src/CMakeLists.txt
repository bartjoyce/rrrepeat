list(APPEND SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/globals.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/globals.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/update.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/update.cpp
)
add_subdirectory(sound)
add_subdirectory(views)
add_subdirectory(util)
set(SOURCES ${SOURCES} PARENT_SCOPE)
