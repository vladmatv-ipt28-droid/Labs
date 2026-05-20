@echo off
echo ==========================================
echo Компіляція Lab6
echo ==========================================

cd /d "C:\Users\pmatv\OneDrive\Desktop\4 семестр\ЗПАД\Lab6"

if not exist build mkdir build
cd build

echo Генерація файлів збірки...
cmake .. -G "MinGW Makefiles"

if %errorlevel% neq 0 (
    echo.
    echo Помилка CMake. Спробуємо альтернативний варіант...
    cmake .. -G "Unix Makefiles"
)

echo.
echo Компіляція...
mingw32-make

if %errorlevel% neq 0 (
    echo.
    echo Спроба з make...
    make
)

echo.
echo ==========================================
echo Компіляція завершена!
echo Запустіть run.bat для запуску програми
echo ==========================================
pause