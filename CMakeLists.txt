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

add_executable(DSA_Project_Fall25 main.cpp
        src/FileIO.cpp
        src/GraphicalUserInterface.cpp
        include/GraphicalUserInterface.h
        src/CommandLineInterface.cpp
        include/CommandLineInterface.hpp
        include/ParseError.hpp
        src/ParseError.cpp
        include/XMLMinifier.hpp
        src/XMLMinifier.cpp
        src/XMLFormatter.cpp
        src/XMLParsser.cpp
        src/XMLNode.cpp
        src/XMLCompressor.cpp
        include/XMLCompressor.hpp
        src/XMLDecompression.cpp
        include/XMLToJson.hpp
        src/XMLToJson.cpp
        src/NetworkAnalyzer.cpp
        src/GraphVisualizer.cpp
        include/GraphVisualizer.h
        include/AppMode.h
        include/XMLNode.hpp
)

set_target_properties(DSA_Project_Fall25 PROPERTIES
        OUTPUT_NAME "xml_editor"
)

if(WIN32)
    get_target_property(_qmake_executable Qt6::qmake IMPORTED_LOCATION)
    get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")

    add_custom_command(TARGET DSA_Project_Fall25 POST_BUILD
            COMMAND "${WINDEPLOYQT_EXECUTABLE}"
            --verbose 1
            --compiler-runtime
            --dir "${CMAKE_CURRENT_BINARY_DIR}"
            "$<TARGET_FILE:DSA_Project_Fall25>"
            COMMENT "Deploying Qt dependencies and MinGW runtimes to build directory..."
    )
endif()

target_link_libraries(DSA_Project_Fall25
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

