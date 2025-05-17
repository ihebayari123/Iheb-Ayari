cmake_minimum_required(VERSION 3.5)

project(StartWise2A17Integration)

set(CMAKE_CXX_STANDARD 11)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/login.cpp
    src/arduin.cpp
    src/database/database_connection.h
)

set(HEADERS
    src/mainwindow.h
    src/login.h
    src/arduin.h
)

set(UI_FILES
    src/login.ui
)

qt5_wrap_ui(UI_HEADERS ${UI_FILES})

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${UI_HEADERS})

target_link_libraries(${PROJECT_NAME} Qt5::Widgets)