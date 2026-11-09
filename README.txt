================================================================================
  VEHICLE RENTAL MANAGEMENT SYSTEM  –  ASSIGNMENT 3
  Qt 6 Windows GUI Application  |  C++17  |  No Qt Designer
================================================================================

OVERVIEW
--------
A fully programmatic Qt 6 Widgets application demonstrating:
  • OOP        – Encapsulation, Inheritance, Polymorphism, Abstraction
  • Qt Widgets – All UI built in C++ (no .ui files, no Qt Designer)
  • Signals & Slots – Button interactions + 3 custom slots
  • Qt Containers   – QList<Vehicle*> fleet management
  • Qt Iterators    – QList::const_iterator used in file I/O, filter, table
  • Qt Generics     – 4 template functions in VehicleUtils.h
  • File Handling   – QFile + QTextStream load/save

MARK ALLOCATION COVERAGE
-------------------------
  Class Design (OOP)         : Vehicle (abstract), Car, Motorcycle          /20
  Qt Widgets UI              : QLabel, QLineEdit, QComboBox, QPushButton,
                               QTableWidget, QSpinBox, QDoubleSpinBox,
                               QGroupBox, QTabWidget, QVBoxLayout,
                               QHBoxLayout, QGridLayout — all programmatic   /20
  Signals & Slots            : All buttons connected via connect();
                               Custom slots: onTypeChanged(),
                               onTableSelectionChanged(), onFilterChanged()  /10
  Qt Containers              : QList<Vehicle*> m_fleet                       /15
  Qt Iterators               : QList::const_iterator in saveToFile(),
                               refreshTable(), onFilterChanged(),
                               onSearchVehicle(), updateSummary()            /10
  Qt Generics (Templates)    : countIf<T,P>(), findFirst<T,P>(),
                               filterList<T,P>(), totalPricePerDay<T>()
                               — all in VehicleUtils.h                       /10
  File Handling              : QFile + QTextStream load on start,
                               save on exit and after every mutation         /10
  Business Logic             : Duplicate ID prevention, rent/return guards,
                               empty-field validation, confirm on remove      /5
  TOTAL                                                                     /100

PROJECT STRUCTURE
-----------------
  VehicleRentalGUI/
  ├── CMakeLists.txt       – Qt6 Widgets build (AUTOUIC OFF – no Designer)
  ├── README.txt
  ├── vehicles.dat         – 30 pre-loaded vehicles (20 cars, 10 motorcycles)
  ├── main.cpp             – QApplication entry point
  ├── Vehicle.h / .cpp     – Abstract base class
  ├── Car.h / .cpp         – Car derived class
  ├── Motorcycle.h / .cpp  – Motorcycle derived class
  ├── VehicleUtils.h       – Template utility functions (Generics requirement)
  └── MainWindow.h / .cpp  – Main window, all UI built programmatically

UI LAYOUT
---------
  Three tabs — all widgets created in C++, zero .ui files:
    Tab 1 "Add Vehicle"    – form with QGridLayout, type-switching spec groups
    Tab 2 "Manage Fleet"   – QTableWidget with search, filter, rent/return/remove
    Tab 3 "Summary"        – live fleet statistics using template functions

FILE FORMAT  (vehicles.dat)
---------------------------
  Car|ID|Brand|Model|PricePerDay|IsRented(0/1)|Doors|Seats
  Motorcycle|ID|Brand|Model|PricePerDay|IsRented(0/1)|EngineCC

BUILD REQUIREMENTS
------------------
  • Qt 6.2+  (Core + Gui + Widgets modules)
  • CMake 3.16+
  • C++17 compiler (MinGW 64-bit recommended on Windows)

BUILD IN QT CREATOR
-------------------
  1. File → Open File or Project → select CMakeLists.txt
  2. Choose kit: Qt 6.x MinGW 64-bit → Configure Project
  3. Press Ctrl+B to build
  4. Press Ctrl+R to run
     (vehicles.dat is auto-copied to the build folder by CMake)

BUILD FROM COMMAND LINE (Windows PowerShell)
--------------------------------------------
  cd VehicleRentalGUI
  mkdir build && cd build
  cmake .. -DCMAKE_PREFIX_PATH="C:\Qt\6.x.x\mingw_64"
  cmake --build . --parallel
  copy ..\vehicles.dat .
  .\VehicleRentalGUI.exe

NOTE ON Qt DESIGNER
-------------------
  CMAKE_AUTOUIC is set to OFF in CMakeLists.txt.
  There are no .ui files anywhere in the project.
  Every widget, layout, and signal/slot connection is written
  directly in MainWindow.cpp as required by the assignment.
================================================================================
