cmake_minimum_required(VERSION 2.8)
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.10" CACHE STRING "Minimum OS X deployment version")
project(RRREPEAT)

set(CMAKE_CXX_STANDARD 14)

set(RRREPEAT_VERSION 1.0.0)

# libsoundio options
set(BUILD_EXAMPLE_PROGRAMS OFF CACHE BOOL "Build example programs")
set(BUILD_TESTS OFF CACHE BOOL "Build tests")

add_subdirectory(src)
add_subdirectory(lib/ddui)
add_subdirectory(lib/libsoundio)

list(APPEND ASSETS
    ${ddui_ASSETS}
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/SFRegular.ttf
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/SFRegularItalic.ttf
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/SFBold.ttf
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/SFBoldItalic.ttf
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/SFMedium.ttf
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/SFMediumItalic.ttf
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/SFThin.ttf
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/PTMono.ttf
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/rhodes.ogg
)

if(APPLE)

    set(ICON ${CMAKE_CURRENT_SOURCE_DIR}/assets/icon.icns)
    set(MACOSX_BUNDLE_ICON_FILE icon.icns)
    set_source_files_properties(${ICON} PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources"
    )

    set_source_files_properties(
        ${ASSETS}
        PROPERTIES
        MACOSX_PACKAGE_LOCATION Resources/assets
    )
    set(MACOSX_BUNDLE_BUNDLE_NAME "RRREPEAT")
    
    add_executable(RRREPEAT MACOSX_BUNDLE ${SOURCES} ${ASSETS} ${ICON})

    set_target_properties(RRREPEAT PROPERTIES
        MACOSX_BUNDLE_ICON_FILE icon.icns
        MACOSX_BUNDLE_INFO_PLIST ${ddui_MACOSX_BUNDLE}
        RESOURCE ${ICON}
    )

else()

    add_executable(RRREPEAT WIN32 ${SOURCES} ${ASSETS})
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/assets/rrrepeat.desktop.in
        ${CMAKE_CURRENT_BINARY_DIR}/rrrepeat.desktop
    )
    list(APPEND ASSETS
        ${CMAKE_CURRENT_SOURCE_DIR}/assets/icon.png
        ${CMAKE_CURRENT_BINARY_DIR}/rrrepeat.desktop
    )
    file(COPY ${ASSETS} DESTINATION "assets/")

endif()

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libsoundio/
)

get_directory_property(LIBSOUNDIO_LIBS DIRECTORY lib/libsoundio DEFINITION LIBSOUNDIO_LIBS)

target_link_libraries(RRREPEAT
    ddui
    libsoundio_static
    ${LIBSOUNDIO_LIBS}
)
