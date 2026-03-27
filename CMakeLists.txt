cmake_minimum_required(VERSION 3.20)
project(ScanForgeCC VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# vcpkg toolchain (set via -DCMAKE_TOOLCHAIN_FILE ou variável de ambiente)
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "vcpkg toolchain file")
endif()

find_package(Qt6 REQUIRED COMPONENTS Widgets)

# --- Biblioteca interna (tudo que não depende de UI) ---
# Usada tanto pelo executável quanto pelos testes
set(LIB_SOURCES
    src/services/system_info_service.cpp
    src/services/cpu_service.cpp
    src/services/memory_service.cpp
    src/services/temperature_service.cpp
    src/services/disk_service.cpp
    src/services/process_service.cpp
    src/models/cpu_info.cpp
    src/models/memory_info.cpp
    src/models/system_info.cpp
    src/models/temperature_info.cpp
    src/models/disk_info.cpp
    src/models/process_info.cpp
    src/utils/formatter.cpp
)

add_library(scanforge_core STATIC ${LIB_SOURCES})
target_include_directories(scanforge_core PUBLIC ${CMAKE_SOURCE_DIR}/include)
if(WIN32)
    target_link_libraries(scanforge_core PUBLIC pdh psapi)
endif()

# --- Executável principal ---
set(APP_SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/widgets/info_card_widget.cpp
    src/widgets/cpu_widget.cpp
    src/widgets/memory_widget.cpp
    src/widgets/system_widget.cpp
    src/widgets/temperature_widget.cpp
    src/widgets/disk_widget.cpp
    src/widgets/process_widget.cpp
    include/mainwindow.h
    include/widgets/info_card_widget.h
    include/widgets/cpu_widget.h
    include/widgets/memory_widget.h
    include/widgets/system_widget.h
    include/widgets/temperature_widget.h
    include/widgets/disk_widget.h
    include/widgets/process_widget.h
)

if(WIN32)
    add_executable(${PROJECT_NAME} WIN32 ${APP_SOURCES})
else()
    add_executable(${PROJECT_NAME} ${APP_SOURCES})
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE scanforge_core Qt6::Widgets)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# --- Testes ---
option(BUILD_TESTS "Compilar testes unitários" ON)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# --- Empacotamento para distribuição ---
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION .)

if(WIN32)
    get_target_property(QT_QMAKE Qt6::qmake IMPORTED_LOCATION)
    get_filename_component(QT_BIN_DIR "${QT_QMAKE}" DIRECTORY)
    find_program(WINDEPLOYQT windeployqt HINTS "${QT_BIN_DIR}")

    if(WINDEPLOYQT)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND "${WINDEPLOYQT}"
                --no-translations
                --no-opengl-sw
                --no-system-d3d-compiler
                --dir "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
                "$<TARGET_FILE:${PROJECT_NAME}>"
            COMMENT "Executando windeployqt para empacotar DLLs..."
        )
    endif()
endif()
