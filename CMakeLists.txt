cmake_minimum_required(VERSION 3.16)
project(VehicleRentalGUI VERSION 2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Qt auto-tools
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC OFF)   # OFF – no Qt Designer .ui files used

# Find Qt6 Widgets (includes Core + GUI automatically)
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
qt_standard_project_setup()

# ── Sources ───────────────────────────────────────────────────────────────────
set(SOURCES
    main.cpp
    Vehicle.cpp
    Car.cpp
    Motorcycle.cpp
    MainWindow.cpp
)

set(HEADERS
    Vehicle.h
    Car.h
    Motorcycle.h
    VehicleUtils.h
    MainWindow.h
)

set(DATA_FILES
    vehicles.dat
)

# ── Executable ────────────────────────────────────────────────────────────────
qt_add_executable(VehicleRentalGUI
    ${SOURCES}
    ${HEADERS}
    ${DATA_FILES}
)

target_link_libraries(VehicleRentalGUI
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)

# ── Auto-copy vehicles.dat to build directory ─────────────────────────────────
configure_file(
    ${CMAKE_SOURCE_DIR}/vehicles.dat
    ${CMAKE_BINARY_DIR}/vehicles.dat
    COPYONLY
)

install(TARGETS VehicleRentalGUI
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
