cmake_minimum_required(VERSION 3.5)

project(DesktopAgenda LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Widgets Network REQUIRED)


add_executable(${PROJECT_NAME}
    main.cpp
    agenda_window.cpp
    agenda_window.h
    add_event_dialog.cpp
    add_event_dialog.h
    login_dialog.h
    login_dialog.cpp
    .gitignore
)

target_link_libraries(${PROJECT_NAME} Qt6::Widgets Qt6::Network)
