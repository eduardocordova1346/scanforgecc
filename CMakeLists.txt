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

set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
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
    src/widgets/info_card_widget.cpp
    src/widgets/cpu_widget.cpp
    src/widgets/memory_widget.cpp
    src/widgets/system_widget.cpp
    src/widgets/temperature_widget.cpp
    src/widgets/disk_widget.cpp
    src/widgets/process_widget.cpp
    src/utils/formatter.cpp
)

set(HEADERS
    include/mainwindow.h
    include/services/system_info_service.h
    include/services/cpu_service.h
    include/services/memory_service.h
    include/services/temperature_service.h
    include/services/disk_service.h
    include/services/process_service.h
    include/models/cpu_info.h
    include/models/memory_info.h
    include/models/system_info.h
    include/models/temperature_info.h
    include/models/disk_info.h
    include/models/process_info.h
    include/widgets/info_card_widget.h
    include/widgets/cpu_widget.h
    include/widgets/memory_widget.h
    include/widgets/system_widget.h
    include/widgets/temperature_widget.h
    include/widgets/disk_widget.h
    include/widgets/process_widget.h
    include/utils/formatter.h
)

# WIN32 flag faz o exe abrir sem janela de console no Windows
if(WIN32)
    add_executable(${PROJECT_NAME} WIN32 ${SOURCES} ${HEADERS})
else()
    add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)

# Libs específicas de plataforma
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE pdh psapi)
endif()

# Warnings úteis
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# --- Empacotamento para distribuição ---
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION .)

# No Windows, windeployqt copia as DLLs do Qt automaticamente
if(WIN32)
    # Localiza o windeployqt que vem com o Qt
    get_target_property(QT_QMAKE Qt6::qmake IMPORTED_LOCATION)
    get_filename_component(QT_BIN_DIR "${QT_QMAKE}" DIRECTORY)
    find_program(WINDEPLOYQT windeployqt HINTS "${QT_BIN_DIR}")

    if(WINDEPLOYQT)
        # Após o build, roda windeployqt para copiar DLLs necessárias
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
