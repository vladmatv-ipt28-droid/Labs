@echo off
echo ==========================================
echo Встановлення необхідних бібліотек для Lab6
echo ==========================================
echo.

echo Перевірка наявності компілятора...
where g++ >nul 2>nul
if %errorlevel% neq 0 (
    echo ❌ g++ не знайдено!
    echo Будь ласка, встановіть MinGW або Visual Studio
    echo.
    echo Для встановлення MinGW:
    echo 1. Скачайте https://github.com/niXman/mingw-builds-binaries/releases
    echo 2. Додайте шлях до bin в PATH
    goto :end
)

echo ✅ g++ знайдено!

echo.
echo Перевірка наявності CMake...
where cmake >nul 2>nul
if %errorlevel% neq 0 (
    echo ❌ CMake не знайдено!
    echo Скачайте та встановіть CMake: https://cmake.org/download/
    echo Під час встановлення оберіть "Add CMake to system PATH"
    goto :end
)

echo ✅ CMake знайдено!

echo.
echo Перевірка наявності OpenCV...
if exist "C:\opencv\build" (
    echo ✅ OpenCV знайдено в C:\opencv
    setx OPENCV_DIR "C:\opencv\build"
) else if exist "C:\Program Files\OpenCV" (
    echo ✅ OpenCV знайдено
) else (
    echo ⚠️ OpenCV не знайдено!
    echo Завантажте OpenCV: https://github.com/opencv/opencv/releases
)

:end
echo.
echo ==========================================
echo Встановлення завершено!
echo Запустіть build.bat для компіляції
echo ==========================================
pause