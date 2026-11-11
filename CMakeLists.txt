cmake_minimum_required(VERSION 3.16.0)

project(kwin_static_blur_dx
    VERSION 2.6.0
    LANGUAGES CXX
)

set(KF_MIN_VERSION "5.240.0")
set(QT_MIN_VERSION "6.6.0")
set(QT_MAJOR_VERSION 6)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CPACK_PACKAGE_NAME "kwin-static-blur-dx")
set(CPACK_PACKAGING_INSTALL_PREFIX "/usr")
set(CPACK_PACKAGE_FILE_NAME "kwin-static-blur-dx")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Fork of kwin-better-blur, which itself is a fork of the default KDE Blur that includes useful additions, with continued development")
include(CPack)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose Release or Debug" FORCE)
endif()
# user build options
option(BBDX_X11 "Build Static Blur DX for KWin X11 instead of KWin Wayland" OFF)
option(BBDX_DEBUG "Enable extra code paths useful for debugging (Warning: major performance impact)" OFF)

# deprecated build options
if(DEFINED BETTERBLUR_X11)
    message(DEPRECATION "The BETTERBLUR_X11 build option is deprecated - use BBDX_X11 instead")
    set(BBDX_X11 ${BETTERBLUR_X11} CACHE BOOL "BETTERBLUR_X11 alias" FORCE)
    unset(BETTERBLUR_X11 CACHE)
endif()
# Dependencies
find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

include(FeatureSummary)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

find_package(Qt6 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS
    Core
    DBus
    Gui
    Network
    OpenGL
    Qml
    Quick
    Widgets
)
include_directories(${Qt6Widgets_INCLUDE_DIRS} ${Qt6Network_INCLUDE_DIRS} ${Qt6OpenGL_INCLUDE_DIRS} ${Qt6Xml_INCLUDE_DIRS})
add_definitions(${Qt6Widgets_DEFINITIONS})

find_package(KF6 ${KF_MIN_VERSION} REQUIRED COMPONENTS
    ColorScheme
    Config
    ConfigWidgets
    CoreAddons
    I18n
    KCMUtils
    WidgetsAddons
    WindowSystem
)

find_package(KDecoration3 REQUIRED)

find_package(epoxy REQUIRED)

find_package(X11 REQUIRED)
find_package(XCB REQUIRED COMPONENTS XCB)
if(BBDX_X11)
    find_package(KWinX11 REQUIRED COMPONENTS kwineffects)
    find_package(KWinX11DBusInterface CONFIG REQUIRED)
    set(KWin_VERSION ${KWinX11_VERSION})
    set(KWin_VERSION_MAJOR ${KWinX11_VERSION_MAJOR})
    set(KWin_VERSION_MINOR ${KWinX11_VERSION_MINOR})
    set(KWin_VERSION_PATCH ${KWinX11_VERSION_PATCH})
else()
    find_package(KWin REQUIRED COMPONENTS kwineffects)
    find_package(KWinDBusInterface CONFIG REQUIRED)
endif()
if(BBDX_X11 OR NOT KWin_VERSION VERSION_EQUAL "6.8.2")
    message(FATAL_ERROR "Static Blur DX currently targets KWin Wayland 6.8.2; detected ${KWin_VERSION}.")
endif()

add_subdirectory(src)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

include(CTest)
if(BUILD_TESTING)
    add_executable(image_source_test tests/image_source_test.cpp)
    target_include_directories(image_source_test PRIVATE src)
    target_link_libraries(image_source_test PRIVATE Qt6::Gui)
    add_test(NAME image_source COMMAND image_source_test)
    add_executable(shader_test tests/shader_test.cpp src/blur.qrc)
    target_include_directories(shader_test PRIVATE src)
    target_link_libraries(shader_test PRIVATE KWin::kwin Qt6::Gui epoxy::epoxy)
    add_test(NAME shaders COMMAND shader_test)
    set_tests_properties(shaders PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen;LIBGL_ALWAYS_SOFTWARE=1" SKIP_RETURN_CODE 77)
endif()
if(BUILD_TESTING)
    set(config_test_SOURCES tests/config_test.cpp)
    kconfig_add_kcfg_files(config_test_SOURCES src/blurconfig.kcfgc)
    add_executable(config_test ${config_test_SOURCES})
    target_include_directories(config_test PRIVATE src ${CMAKE_CURRENT_BINARY_DIR}/src)
    target_link_libraries(config_test PRIVATE KF6::ConfigGui)
    add_test(NAME config COMMAND config_test)
endif()
