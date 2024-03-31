# TCP Mirror messages server

## Task description

1. На основе библиотеки libev реализовать неблокирующий  TCP-сервер под ОС на базе ядра Linux со следующим функционалом:
    • Ожидание подключения на 5000/TCP порт или Unix-сокет
    • Получаемые данные сервером, должны отправляться обратно клиенту
    • Каждое подключение должно логироваться записью в log-file(дата, время, полученные данные)

2. Написать bash скрипт для тестирования сервера с установлением одновременно 20 подключений.
3. Организовать сборку реализованного сервера через cmake

Дополнительно (не обязательно, но будет плюсом):
1. Организовать упаковку приложения в deb-пакет
2. Создать .gitlab-ci.yml с описанием сборки deb-пакета
3. Создать Dockerfile для развертывания сервера в Docker контейнере

## Build binary

From project directory
```
mkdir -p build
cd build
cmake ...
make
```

## Build DEB package

Prepare on Ubuntu (tested on 22.04)

```sudo apt install ninja-build```

Build DEB from project directory

```
mkdir -p build
cd build
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
cpack -G DEB
```

See deb packege in direcrory ```_packages/```