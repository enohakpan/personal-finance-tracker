@echo off
setlocal

cd /d "%~dp0"

echo Compiling Personal Finance Tracker...
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp Transaction.cpp FinanceManager.cpp InputHelper.cpp -o finance_tracker.exe

if errorlevel 1 (
    echo.
    echo Build failed. Fix compile errors and run this script again.
    exit /b 1
)

echo.
echo Running app...
echo.
finance_tracker.exe

endlocal