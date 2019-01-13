cmake_minimum_required(VERSION 3.13)
project(Uni_Dubna_Commander)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -g0 -Wall")
set(CMAKE_CXX_FLAGS_DEBUG  "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g3 -Wall")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 COMPONENTS Widgets Network)

include_directories(${Qt5Widgets_INCLUDE_DIRS} ${Qt5Network_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS} ${Qt5Network_EXECUTABLE_COMPILE_FLAGS}")
add_definitions(${Qt5Widgets_DEFINITIONS} ${Qt5Network_DEFINITIONS})

add_executable(${PROJECT_NAME} main.cpp Widgets/LoginWidget/LoginWidget.cpp Network/NetworkManager.cpp Widgets/MainWindow/MainWindow.cpp Libs/tinyxml2/tinyxml2.cpp FolderItem.cpp Libs/GZip/GZip.cpp Widgets/ProgressWidget/ProgressWidget.cpp Widgets/TreeWidgetItem.cpp Constants.cpp)

target_link_libraries(${PROJECT_NAME} ${Qt5Widgets_LIBRARIES} ${Qt5Network_LIBRARIES} z)
