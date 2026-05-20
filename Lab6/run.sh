@echo off
echo ==========================================
echo Запуск Lab6
echo ==========================================

cd /d "C:\Users\pmatv\OneDrive\Desktop\4 семестр\ЗПАД\Lab6"

if exist build\lab6.exe (
    build\lab6.exe
) else if exist build\Release\lab6.exe (
    build\Release\lab6.exe
) else if exist lab6.exe (
    lab6.exe
) else (
    echo ❌ Не знайдено виконуваний файл!
    echo Спочатку запустіть build.bat
    pause
    exit /b 1
)

echo.
echo Програма завершила роботу
pause