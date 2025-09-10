<div align="center">
  
  # 🛡️ Реализация алгоритма кеширования на языке С++
  
  ![GitHub code size](https://img.shields.io/github/languages/code-size/BulgakovDmitry/readmeRepo)  
  
  <img src="docs/LFUPicREADME.png" alt="LFU Banner" width="800">
  
</div>

## Другие языки

1. [Russian](/README.md)
2. [English](/README-en.md)

## 🚀 Установка и сборка:

Для установки, компиляции и запуска с флагом “--interface” введите:
```cpp
git clone https://github.com/BulgakovDmitry/LFU_cache
cd LFU_cache
make run_with_interface
```
Для компиляции и запуска без интерфейса замените make run_with_interface на
```cpp
make run
```

Программа также имеет встроенную систему тестирования, которая при ошибке выдает подробные логи с информацией.

Для запуска встроенных тестов введите:
```cpp
make test
```

Для просмотра основных целей makefile с кратким описанием введите:
```cpp
make help
```

## ✨ Важно
В программе реализован графический дамп при помощи graphviz.

Для быстрой его установки на линукс в командной строке введите:
```bash
sudo apt update
sudo apt install graphviz
```

## 💡 Немного о дампе
При помощи graphviz мы можем получить понятную и удобную информацию о состоянии кеша и проследить за его изменением при обращении к элементам. Посмотрим в качестве примера, как кешируются элементы типа int:

<div align="center">
  <img src="docs/entering.png" alt="Entering Banner" width="400">
</div>

Графический дамп генерирует следующее изображение:

<div align="center">
  <img src="docs/dump.png" alt="Dump Banner" width="1200">
</div>
