execute_process(COMMAND
    git describe --tags
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    OUTPUT_VARIABLE RRREPEAT_GIT_VERSION
    ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE
)
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/version.cpp.in
    ${CMAKE_CURRENT_BINARY_DIR}/version.cpp
)
list(APPEND SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/globals.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/globals.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/update.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/update.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/version.hpp
    ${CMAKE_CURRENT_BINARY_DIR}/version.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/stb_vorbis.c
    ${CMAKE_CURRENT_SOURCE_DIR}/StereoBuffer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/audio_client.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/audio_client.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/load_ogg.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/load_ogg.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/write_callback.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/write_callback.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/render_waveform.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/render_waveform.cpp
)
set(SOURCES ${SOURCES} PARENT_SCOPE)
