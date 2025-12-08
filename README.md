# C++ Calculator with Plugins (C++17)

![C++17](https://img.shields.io/badge/C%2B%2B-17-00599C.svg?style=flat-square&logo=cplusplus&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-3.14%2B-0672A4.svg?style=flat-square&logo=cmake&logoColor=white)

## Основные возможности
- Базовые и продвинутые операции:
  - Модульность: Все математические операции загружаются динамически из папки `./plugins`.
  - Арифметика: `+`, `-`, `*`, `/`, `^` (степень);
  - Тригонометрия: `sin`, `cos`, `tg`, `ctg`;
  - Другие: `ln`, `!n`, `n!`, `rad`, `deg`.
- Поддержка платформ: Кроссплатформенная сборка и запуск на **Linux** и **Windows** (MinGW).

## Сборка и Запуск
### Требования
- Компилятор с поддержкой C++17 (GCC, Clang, MSVC/MinGW)
- CMake 3.14+

### Инструкция по сборке

#### Linux / macOS

``` Bash
# 1. Клонирование
git clone https://github.com/ilindan-dev/Calculator
cd Calculator

# 2. Конфигурация и сборка
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

#### Windows (MinGW / Visual Studio)
```PowerShell
# 1. Клонирование
git clone https://github.com/ilindan-dev/Calculator
cd Calculator

# 2. Конфигурация
cmake -B build

# 3. Сборка
cmake --build build --config Release
```

### Запуск и Тестирование
После сборки исполняемый файл и плагины будут находиться в директории `build/app` (или `build/app/Release` на Windows).
```Bash
# Запуск калькулятора
./build/app/calculator_app -e "2 + 2 * 2"

# Запуск тестов (GoogleTest)
cd build
ctest --output-on-failure
```