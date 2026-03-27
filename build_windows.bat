@echo off
REM ==========================================================
REM  ScanForge CC — Script de build para Windows
REM  Gera o .exe pronto para distribuicao em dist\ScanForgeCC\
REM
REM  Requisitos:
REM    - Qt 6 instalado com MinGW (vem junto no instalador do Qt)
REM    - Nao precisa do Visual Studio
REM
REM  Uso:
REM    build_windows.bat C:\Qt\6.8.0\mingw_64 C:\Qt\Tools\mingw1310_64
REM ==========================================================

if "%~1"=="" (
    echo Uso: build_windows.bat ^<caminho-qt^> ^<caminho-mingw^>
    echo.
    echo Exemplo com MinGW (recomendado):
    echo   build_windows.bat C:\Qt\6.8.0\mingw_64 C:\Qt\Tools\mingw1310_64
    echo.
    echo Exemplo com MSVC (requer Visual Studio):
    echo   build_windows.bat C:\Qt\6.8.0\msvc2022_64
    exit /b 1
)

set QT_PATH=%~1

REM Detecta se e MinGW ou MSVC
if "%~2"=="" (
    echo [*] Modo MSVC detectado
    set GENERATOR=Ninja
    set BUILD_MODE=msvc
) else (
    echo [*] Modo MinGW detectado
    set MINGW_PATH=%~2
    set PATH=%MINGW_PATH%\bin;%PATH%
    set GENERATOR=MinGW Makefiles
    set BUILD_MODE=mingw
)

REM Adiciona CMake do Qt ao PATH se existir
if exist "%QT_PATH%\..\..\Tools\CMake_64\bin" (
    set PATH=%QT_PATH%\..\..\Tools\CMake_64\bin;%PATH%
)

echo.
echo [1/4] Configurando CMake com Qt em: %QT_PATH%
cmake -B build -G "%GENERATOR%" -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="%QT_PATH%"
if errorlevel 1 (
    echo ERRO: cmake configure falhou.
    echo Verifique se o caminho do Qt e MinGW estao corretos.
    exit /b 1
)

echo.
echo [2/4] Compilando...
cmake --build build --config Release
if errorlevel 1 (
    echo ERRO: build falhou.
    exit /b 1
)

echo.
echo [3/4] Criando pasta de distribuicao...
if exist dist\ScanForgeCC rmdir /s /q dist\ScanForgeCC
mkdir dist\ScanForgeCC
copy build\ScanForgeCC.exe dist\ScanForgeCC\

echo.
echo [4/4] Copiando DLLs do Qt com windeployqt...
"%QT_PATH%\bin\windeployqt.exe" --no-translations --no-opengl-sw --no-system-d3d-compiler dist\ScanForgeCC\ScanForgeCC.exe
if errorlevel 1 (
    echo AVISO: windeployqt falhou. Copiando DLLs manualmente...
    copy build\*.dll dist\ScanForgeCC\ 2>nul
    xcopy build\platforms dist\ScanForgeCC\platforms\ /s /i 2>nul
    xcopy build\styles dist\ScanForgeCC\styles\ /s /i 2>nul
)

REM Se MinGW, copia as DLLs do runtime
if "%BUILD_MODE%"=="mingw" (
    copy "%MINGW_PATH%\bin\libgcc_s_seh-1.dll" dist\ScanForgeCC\ 2>nul
    copy "%MINGW_PATH%\bin\libstdc++-6.dll" dist\ScanForgeCC\ 2>nul
    copy "%MINGW_PATH%\bin\libwinpthread-1.dll" dist\ScanForgeCC\ 2>nul
)

echo.
echo ============================================
echo   BUILD COMPLETO!
echo   Distribuivel em: dist\ScanForgeCC\
echo   Execute: dist\ScanForgeCC\ScanForgeCC.exe
echo ============================================
