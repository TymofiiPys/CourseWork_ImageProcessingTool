cmake_minimum_required(VERSION 3.5)

project(CourseWork_ImageProcessingTool VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Eigen3 3.3 REQUIRED)

set(PROJECT_SOURCES
        src/main.cpp
        src/frontend/mainwindow.cpp
        src/frontend/mainwindow.h
        src/frontend/mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(CourseWork_ImageProcessingTool
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        README.md
        MDTutorial.md
        .gitignore
        src/frontend/recentimages.h
        src/frontend/recentimages.cpp
        src/config/iptconfigmanager.h src/config/iptconfigmanager.cpp
        src/imgproc/invert.h
        src/imgproc/invert.cpp
        src/frontend/imageprocessorwrapper.h src/frontend/imageprocessorwrapper.cpp
        src/frontend/rotatedialog.h src/frontend/rotatedialog.cpp src/frontend/rotatedialog.ui
        src/imgproc/rotate.h
        src/imgproc/rotate.cpp
        src/imgproc/mirror.h
        src/imgproc/mirror.cpp
        src/imgproc/aliases.h
        src/imgproc/togray.h
        src/imgproc/togray.cpp
        src/imgproc/histeq.h
        src/imgproc/histeq.cpp
        src/frontend/choosechanneldialog.h src/frontend/choosechanneldialog.cpp src/frontend/choosechanneldialog.ui
        src/imgproc/log_transform.h
        src/imgproc/log_transform.cpp
        src/frontend/ltdialog.h src/frontend/ltdialog.cpp src/frontend/ltdialog.ui
        src/imgproc/averaging.h
        src/imgproc/averaging.cpp
        src/imgproc/gauss_filter.h
        src/imgproc/gauss_filter.cpp
        src/imgproc/exp_transform.h
        src/imgproc/exp_transform.cpp
        src/frontend/etdialog.h src/frontend/etdialog.cpp src/frontend/etdialog.ui
        src/imgproc/scale.h
        src/imgproc/scale.cpp
        src/frontend/trdialog.h src/frontend/trdialog.cpp src/frontend/trdialog.ui
        src/imgproc/box_filter.h
        src/imgproc/box_filter.cpp
        src/imgproc/laplacian.h
        src/imgproc/laplacian.cpp
        src/frontend/bfdialog.h src/frontend/bfdialog.cpp src/frontend/bfdialog.ui
        src/frontend/gfdialog.h src/frontend/gfdialog.cpp src/frontend/gfdialog.ui
        src/frontend/laplfiltdialog.h src/frontend/laplfiltdialog.cpp src/frontend/laplfiltdialog.ui

    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET CourseWork_ImageProcessingTool APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(CourseWork_ImageProcessingTool SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(CourseWork_ImageProcessingTool
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(CourseWork_ImageProcessingTool PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Eigen3::Eigen)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.CourseWork_ImageProcessingTool)
endif()
set_target_properties(CourseWork_ImageProcessingTool PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS CourseWork_ImageProcessingTool
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(CourseWork_ImageProcessingTool)
endif()
