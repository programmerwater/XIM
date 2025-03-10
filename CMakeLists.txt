cmake_minimum_required(VERSION 3.3)
project(XIM VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../XIM_xBin")
add_executable(XIM)
add_subdirectory(XFramelessWidget)

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
target_link_libraries(
    XIM PRIVATE
    Qt5::Core
	Qt5::Gui
	Qt5::Widgets

    xframelesswidget
)
target_include_directories(
    XIM PRIVATE
    .
)

set_target_properties(
    XIM PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
)

target_compile_definitions(
	XIM PRIVATE
	-DX_WIDGET_SHARED
)

target_compile_options(
	XIM PRIVATE
	#/W4
	$<$<CXX_COMPILER_ID:MSVC>:/WX /utf-8>
	$<$<CXX_COMPILER_ID:MSVC>:$<$<CONFIG:DEBUG>:/WX>>
	#$<$<CXX_COMPILER_ID:MSVC>:$<$<NOT:$<CONFIG:DEBUG>>:/WX->>

    #$<$<CXX_COMPILER_ID:GNU>:-Wall -Werror>
)

set(Headers
	context/message.h
	widgets/mainwgt.h
	widgets/chattextbrowser.h
	widgets/chattexteditor.h
    widgets/chattexteditortool.h
)

set(Srcs
    main.cpp
    widgets/mainwgt.cpp
	widgets/chattextbrowser.cpp
	widgets/chattexteditor.cpp
    widgets/chattexteditortool.cpp
)

target_sources(
	XIM PRIVATE
    ${Headers}
    ${Srcs}
)

set_target_properties(
    XIM PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)

target_include_directories(
    XIM PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

set_target_properties(
    XIM PROPERTIES
    OUTPUT_NAME "XIM"
    PREFIX ""
    IMPORT_PREFIX ""
    DEBUG_POSTFIX "d"
)
