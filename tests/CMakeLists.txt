include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.2
)

# Evita que o gtest instale junto com o projeto
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Testes
add_executable(scanforge_tests
    test_formatter.cpp
    test_models.cpp
    test_services.cpp
)

target_link_libraries(scanforge_tests PRIVATE
    scanforge_core
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(scanforge_tests)
