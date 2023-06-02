cmake_minimum_required(VERSION 3.5)

project(fssp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

set(TS_FILES translation/fssp_ru_RU.ts)

qt_create_translation(QM_FILES ${PROJECT_SOURCE_DIR} ${TS_FILES})

set(PROJECT_INCLUDES
        include/mainwindow.h
        include/signal.h
        include/filereader.h
        include/customlabel.h
        include/waveformsdialog.h
        include/graphlabel.h
)

set(PROJECT_SOURCES
        src/main.cpp
        src/mainwindow.cpp
        src/signal.cpp
        src/filereader.cpp
        src/customlabel.cpp
        src/waveformsdialog.cpp
        src/graphlabel.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(fssp
        MANUAL_FINALIZATION
        ${PROJECT_INCLUDES}
        ${PROJECT_SOURCES}
        ${QM_FILES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET fssp APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(fssp SHARED
            ${PROJECT_INCLUDES}
            ${PROJECT_SOURCES}
            ${QM_FILES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(fssp
            ${PROJECT_INCLUDES}
            ${PROJECT_SOURCES}
            ${QM_FILES}
        )
    endif()
endif()

target_include_directories(fssp PRIVATE
    src
    include
)

target_link_libraries(fssp PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(fssp PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS fssp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(fssp)
endif()
