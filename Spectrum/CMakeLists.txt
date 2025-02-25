cmake_minimum_required(VERSION 3.16)

project(Spectrum VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)


set(PROJECT_SOURCES
    main.cpp
    Spectrum.cpp
    Spectrum.h
)


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Spectrum
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ../Source/Functions/SPFileIO.cpp
        ../Source/Functions/SPFileIO.h
        ../Source/Lexer/AlifComplete.cpp
        ../Source/Lexer/AlifComplete.h
        ../Source/Lexer/AlifLexer.cpp
        ../Source/Lexer/AlifLexer.h
        ../Source/MenuBar/SPMenu.cpp
        ../Source/MenuBar/SPMenu.h
        ../Source/TextEditor/SPEditor.cpp
        ../Source/TextEditor/SPEditor.h
        ../Source/TextEditor/SPHighlighter.cpp
        ../Source/TextEditor/SPHighlighter.h
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Spectrum APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Spectrum SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Spectrum
            ${PROJECT_SOURCES}
        )
    endif()
endif()


target_include_directories(Spectrum
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../Source/TextEditor
    ${CMAKE_CURRENT_SOURCE_DIR}/../Source/Functions
    ${CMAKE_CURRENT_SOURCE_DIR}/../Source/Lexer
    ${CMAKE_CURRENT_SOURCE_DIR}/../Source/MenuBar
)

target_link_libraries(Spectrum PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Spectrum)
endif()
set_target_properties(Spectrum PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Spectrum
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Spectrum)
endif()
