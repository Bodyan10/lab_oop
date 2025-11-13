#ifndef CONTAINER_H
#define CONTAINER_H

#include <vector>
#include <cstdio>

template <class T>
class Container {
public:
    // Конструкторы
    Container();
    Container(std::vector<T*>& storage);
    Container(const Container& other);
    Container(int size);
    Container& operator=(const Container& other);

    // Геттеры
    int getCount();
    T* getObject(int index);

    // Добавление элементов
    void addObject(T* new_object);
    void setObject(int index, T* new_object);

    // Удаление элементов
    void removeAt(int index);
    void removeElement(T* element);
    void removeSelected();

    // Дополнительные методы для совместимости
    bool hasElement(const T* el);
    void clear();
    T& at(size_t index);
    size_t size();
    T& operator[](size_t index);

    // Деструктор
    ~Container();

private:
    std::vector<T*> myStorage;
};

// Реализация шаблонного класса

template<class T>
Container<T>::Container() {
    printf("Container()\n");
}

template<class T>
Container<T>::Container(std::vector<T*>& storage) {
    printf("Container(std::vector<T*>&)\n");
    for (T* object : storage) {
        myStorage.push_back(new T(*(object)));
    }
}

template<class T>
Container<T>::Container(int size) {
    printf("Container(int size)\n");
    for (int i = 0; i < size; i++) {
        myStorage.push_back(new T());
    }
}

template<class T>
Container<T>::Container(const Container& other) {
    printf("Container(const Container& other)\n");
    for (T* object : other.myStorage) {
        myStorage.push_back(new T(*(object)));
    }
}

template<class T>
Container<T>& Container<T>::operator=(const Container& other) {
    printf("operator=(const Container& other)\n");

    if (this == &other) {
        return *this;
    }

    // 1. Очищаем старые данные
    for (T* object : myStorage) {
        delete object;
    }
    myStorage.clear();

    // 2. Копируем новые данные
    for (T* object : other.myStorage) {
        myStorage.push_back(new T(*object));
    }

    return *this;
}

// Геттеры
template<class T>
int Container<T>::getCount() {
    return myStorage.size();
}

template<class T>
T* Container<T>::getObject(int index) {
    if (index >= 0 && index < getCount()) {
        return myStorage[index];
    } else {
        printf("Попытка получить объект по несуществующему индексу: %d\n", index);
        return nullptr;
    }
}

// Добавление элементов
template<class T>
void Container<T>::addObject(T* new_object) {
    myStorage.push_back(new_object);
    printf("Объект добавлен в контейнер. Всего объектов: %d\n", getCount());
}

template<class T>
void Container<T>::setObject(int index, T* new_object) {
    // Если индекс равен текущему размеру, добавляем новый элемент
    if (index == getCount()) {
        myStorage.push_back(new_object);
        printf("Объект добавлен в конец контейнера\n");
    }
    // Если индекс в пределах существующего массива, заменяем элемент
    else if (index >= 0 && index < getCount()) {
        delete myStorage[index];
        myStorage[index] = new_object;
        printf("Объект заменен по индексу: %d\n", index);
    } else {
        printf("Попытка установить объект по несуществующему индексу: %d\n", index);
    }
}

// Удаление элементов
template<class T>
void Container<T>::removeAt(int index) {
    if (index < 0 || index >= getCount()) {
        printf("Попытка удаления по несуществующему индексу: %d\n", index);
        return;
    }

    // удаляем объект
    if (myStorage[index]){
        delete myStorage[index];
    }

    // Удаляем указатель из вектора
    myStorage.erase(myStorage.begin() + index);
    printf("Объект удален по индексу: %d. Осталось объектов: %d\n", index, getCount());
}

template<class T>
void Container<T>::removeSelected() {
    int removedCount = 0;
    for (int i = getCount() - 1; i >= 0; i--) {
        if (myStorage[i]->isSelected()) {
            delete myStorage[i];
            myStorage.erase(myStorage.begin() + i);
            removedCount++;
        }
    }
    printf("Удалено выделенных объектов: %d. Осталось объектов: %d\n", removedCount, getCount());
}

template<class T>
void Container<T>::removeElement(T* element) {
    for (int i = 0; i < getCount(); i++) {
        if (getObject(i) == element) {
            removeAt(i);
            return;
        }
    }
}

// Дополнительные методы для совместимости
template<class T>
bool Container<T>::hasElement(const T* el) {
    for (const T* obj : myStorage) {
        if (obj == el) return true;
    }
    return false;
}

template<class T>
void Container<T>::clear() {
    for (T* object : myStorage) {
        if (object) {
            delete object;
        }
    }
    myStorage.clear();
    printf("Контейнер очищен\n");
}

template<class T>
T& Container<T>::at(size_t index) {
    return *myStorage[index];
}

template<class T>
size_t Container<T>::size() {
    return myStorage.size();
}

template<class T>
T& Container<T>::operator[](size_t index) {
    return *myStorage[index];
}

// Деструктор
template<class T>
Container<T>::~Container() {
    for (T* object : myStorage) {
        if (object) {
            delete object;
        }
    }
    printf("~Container()\n");
}

#endif // CONTAINER_H
