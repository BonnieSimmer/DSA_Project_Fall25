cmake_minimum_required(VERSION 4.0)
project(DSA_Project_Fall25)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(DSA_Project_Fall25
     main.cpp
    src/FileIO.cpp
    src/XMLMinifier.cpp
    src/XMLFormatter.cpp

)
target_link_libraries(DSA_Project_Fall25
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

