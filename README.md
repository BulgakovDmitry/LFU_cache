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

Также реализованы различные консольные дампы, дающие информацию о состоянии кеша в удобном для дебага формате (в зависимости от текущего объекта дебага).
```cpp
template<typename KeyType, typename ValueType>
void        consoleDump           (const LFU<KeyType, ValueType>& cache);

template<typename KeyType, typename ValueType>
void        consoleGeneralizedDump(const LFU<KeyType, ValueType>& cache);

template<typename KeyType, typename ValueType>
void        consoleFullDump       (const std::vector<ValueType>& vec, std::size_t cacheSize);

template<typename KeyType, typename ValueType>
void        graphDump             (const std::vector<ValueType>& vec, std::size_t cacheSize);
```

## 🔍 Обзор
Определение класса кеша:
```cpp
template<typename KeyType, typename ValueType> 
class LFU {
public:
    explicit LFU(std::size_t cacheSize_) noexcept
        : cacheSize(cacheSize_), tick(0), data{}, numberOfHits(0), hashTable{} {}

    ~LFU() = default;

    using ListIt      = typename std::list<CacheCell<KeyType, ValueType>>::iterator;
    using ListConstIt = typename std::list<CacheCell<KeyType, ValueType>>::const_iterator;

    inline void        cacheHit       ()       noexcept {        ++numberOfHits; }
    inline std::size_t nextTick       ()       noexcept { return ++tick;         }
    inline std::size_t getNumberOfHits() const noexcept { return numberOfHits;   }
    inline std::size_t getCacheSize   () const noexcept { return cacheSize;      }
    inline std::size_t dataSize       () const noexcept { return data.size ();   }
    inline bool        empty          () const noexcept { return data.empty();   }

    inline void push_front(const CacheCell<KeyType, ValueType>& value) { data.push_front(value); }

    inline ListIt       begin()       noexcept { return data.begin (); }
    inline ListConstIt  begin() const noexcept { return data.cbegin(); }
    inline ListIt       end  ()       noexcept { return data.end   (); }
    inline ListConstIt  end  () const noexcept { return data.cend  (); }

    inline void splice(ListIt pos, LFU& other, ListIt it) noexcept { data.splice(pos, other.data, it); }

    inline ListIt      find(const KeyType& key) {
        auto hit = hashTable.find(key);
        return (hit == hashTable.end()) ? data.end() : hit->second;
    }

    inline ListConstIt find(const KeyType& key) const {
        auto hit = hashTable.find(key);
        return (hit == hashTable.end()) ? data.end() : hit->second;
    }

    inline void indexKeyIt(const KeyType& key, ListIt it) { hashTable[key] = it;  }
    inline void eraseKey  (const KeyType& key)            { hashTable.erase(key); }
    inline void eraseIt   (ListIt it) {
        if (it == data.end()) return;
        hashTable.erase(it->key);
        data.erase(it);
    }

private:
    std::size_t                              cacheSize;
    Tick_t                                   tick;
    std::list<CacheCell<KeyType, ValueType>> data;
    std::size_t                              numberOfHits;
    std::unordered_map<
        KeyType, 
        ListIt>                              hashTable;
};
```
Тут определение ячейки кеша имеет следующий вид:
```cpp
template<typename KeyType, typename ValueType> 
struct CacheCell {
    KeyType     key;
    ValueType   value;

    std::size_t numberOfRequests;
    Tick_t      lastAccessedTime;

    bool        emptyFlag;
};
```

Некоторые базовые детали для понимания алгоритма кеширования:

| переменная       | значение                        |
|-------------------------|-----------------------------------|
| `cacheSize`             | число кеш-ячеей в текущем кеше     |
| `tick`             | глобальная переменная кеша, характеризующая время работы алгоритма (в единицах количества обращений)       |
| `data`        | Список кеш-ячеек с данными |
| `key`        | ключ кешируемого значения   |
| `value`      | кешируемое значение      |
| `numberOfRequests`      | количество обращений к текущей кеш-ячейке       |
| `lastAccessedTime`      | время последнего вызова в единицах, характеризующих время работы кеша        |
| `emptyFlag`      | специальный флаг, который определяет, свободна ли текущая ячейка кэша       |
| `numberOfHits` | количество попаданий в кеш |
| `hashTable` | хеш-таблица для быстрого поиска элемента по ключу |

## 📂 Структура проекта
```
LFU/
├── headers/            # Header files
├── src/                # Source code
│
├── tests/              # built-in program testing
│   ├── src/            # test source code
│   └── headers/        # test header files
│
├── graphDump/          # for images and graphviz files
│
├── build/              # Project build folder
│   ├── obj/            # object files
│   └── bin/            # executable files
│
├── myLib/              # my castom library
│
└── Makefile/           # Makefile           
```
