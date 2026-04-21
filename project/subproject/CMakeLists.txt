cmake_minimum_required(VERSION 3.16)

project(Subproject VERSION 1.0.1 LANGUAGES CXX)

set(USE_QT OFF)

if(USE_QT)
    # 开启Qt预编译
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)

    # 查找Qt库
    find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

    # 添加qrc文件
    # qt5_add_resources(QRC_A qrc.qrc)

    # 将src下的ui文件加入工程
    file(GLOB_RECURSE PROJECT_UI_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.ui"
    )
endif()

# 最低C++版本要求
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 只在ninja构建中可用，输出compile_commands.json，用于dangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/Release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_SOURCE_DIR}/bin/Release)

# 设置预处理宏
# add_compile_definitions("A" "B")

# 将src下的文件加入工程
file(GLOB_RECURSE PROJECT_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
    "${CMAKE_SOURCE_DIR}/common/*.h"
    "${CMAKE_SOURCE_DIR}/common/*.cpp"
    "${CMAKE_SOURCE_DIR}/common/*.c"
    "${CMAKE_SOURCE_DIR}/common/*.hpp"
)
# 排除指定文件的添加
# list(FILTER PROJECT_SOURCES EXCLUDE REGEX "*/test/*")

if(WIN32)
    # 获取当前日期
    string(TIMESTAMP BUILD_DATE "%Y%m%d")
    string(TIMESTAMP BUILD_YEAR "%Y")

    # 替换占位符生成最终的.rc文件
    # configure_file("${CMAKE_SOURCE_DIR}/cmake/app.rc.in" app.rc @ONLY)
    # set(RC_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/app.rc")

    # 设置运行时库为 /MT（多线程静态库）
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")

    # 设置应用程序图标
    # set(APP_ICON "${CMAKE_CURRENT_SOURCE_DIR}/app.ico")
    # file(APPEND ${RC_SOURCES} "IDI_ICON1 ICON \"${APP_ICON}\"")
endif()

if(USE_QT)
    if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
        qt_add_executable(${PROJECT_NAME}
            MANUAL_FINALIZATION
            ${PROJECT_SOURCES}
            ${PROJECT_UI_SOURCES}
        )
        # Define target properties for Android with Qt 6 as:
        #    set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
        #                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
        # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
    else()
        if(ANDROID)
            add_library(${PROJECT_NAME} SHARED
                ${PROJECT_SOURCES}
                ${PROJECT_UI_SOURCES}
            )
        # Define properties for Android with Qt 5 after find_package() calls as:
        #    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
        else()
            # 输出为可执行程序
            add_executable(${PROJECT_NAME}
                ${PROJECT_SOURCES}
                ${PROJECT_UI_SOURCES}
                # ${RC_SOURCES}
                # ${QRC}
            )
        endif()
    endif()

    # 链接Qt库
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

    # Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
    # If you are developing for iOS or macOS you should consider setting an
    # explicit, fixed bundle identifier manually though.
    if(${QT_VERSION} VERSION_LESS 6.1.0)
        set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.${PROJECT_NAME})
    endif()
    set_target_properties(${PROJECT_NAME} PROPERTIES
        ${BUNDLE_ID_OPTION}
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
    )

    include(GNUInstallDirs)
    install(TARGETS ${PROJECT_NAME}
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

    if(QT_VERSION_MAJOR EQUAL 6)
        qt_finalize_executable(${PROJECT_NAME})
    endif()
else()
    # 输出为可执行程序
    add_executable(${PROJECT_NAME}
        ${PROJECT_SOURCES}
        # ${RC_SOURCES}
    )
endif()

# 设置头文件包含目录
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/common
    ${CMAKE_SOURCE_DIR}/common/config
    ${CMAKE_SOURCE_DIR}/3rdparty
    # ${CMAKE_SOURCE_DIR}/3rdparty/cef
    ${CMAKE_SOURCE_DIR}/3rdparty/json
    ${CMAKE_SOURCE_DIR}/3rdparty/spdlog
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# 设置依赖库包含目录
target_link_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/3rdparty/spdlog/lib
)

# 设置依赖库
target_link_libraries(${PROJECT_NAME} PRIVATE
    #cef_sandbox.lib
    # libcef_dll_wrapper.lib
    # libcef.lib
    #spdlog.lib
)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    # 使_cplusplus宏能区分c++版本
    if(CMAKE_CXX_STANDARD GREATER_EQUAL 17)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:__cplusplus")
    endif()

    # 设置多核编译标志
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")

    # 设置字符编码格式
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")

    # 在C++20中可以使用u8标记
    # if(CMAKE_CXX_STANDARD GREATER_EQUAL 20)
    #     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:char8_t-")
    # endif()

    # 设置管理员权限
    # set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/MANIFESTUAC(\"level='requireAdministrator' uiAccess='false'\") /SUBSYSTEM:WINDOWS")

    # 隐藏cmd窗口(做windows窗口程序需要配置，winmain入口)
    # set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
endif()

# # 每次构建后自动拷贝
# add_custom_command(
#     TARGET ${PROJECT_NAME} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy
#     $<TARGET_FILE:${PROJECT_NAME}>
#     ${CMAKE_SOURCE_DIR}/output/bin/

#     COMMAND ${CMAKE_COMMAND} -E copy
#     $<TARGET_PDB_FILE:${PROJECT_NAME}>
#     ${CMAKE_SOURCE_DIR}/output/debug/
# )