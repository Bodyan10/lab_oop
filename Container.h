#ifndef CONTAINER_H
#define CONTAINER_H

#include <vector>
#include <cstdio>

template <class T>
class Container {
public:
    Container();
    Container(const std::vector<T*>& storage);
    Container(const Container& other);
    Container(int size);
    Container& operator=(const Container& other);

    int getCount() const;
    T* getObject(int index);

    void addObject(T* new_object);
    void setObject(int index, T* new_object);

    void removeAt(int index);
    void removeSelected();

    std::vector<T*> getAllObjects() const;
    std::vector<T*> getSelectedObjects() const;
    void clear();

    ~Container();

private:
    std::vector<T*> myStorage;
};

template<class T>
Container<T>::Container() {
    printf("Контейнер создан (пустой)\n");
}

template<class T>
Container<T>::Container(const std::vector<T*>& storage) {
    printf("Контейнер создан из вектора\n");
    for (T* object : storage) {
        myStorage.push_back(new T(*object));
    }
}

template<class T>
Container<T>::Container(int size) {
    printf("Контейнер создан с размером %d\n", size);
    for (int i = 0; i < size; i++) {
        myStorage.push_back(new T());
    }
}

template<class T>
Container<T>::Container(const Container& other) {
    printf("Контейнер скопирован\n");
    for (T* object : other.myStorage) {
        myStorage.push_back(new T(*object));
    }
}

template<class T>
Container<T>& Container<T>::operator=(const Container& other) {
    printf("Контейнер присвоен\n");

    if (this == &other) {
        return *this;
    }

    for (T* object : myStorage) {
        delete object;
    }
    myStorage.clear();

    for (T* object : other.myStorage) {
        myStorage.push_back(new T(*object));
    }

    return *this;
}

template<class T>
int Container<T>::getCount() const {
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

template<class T>
void Container<T>::addObject(T* new_object) {
    myStorage.push_back(new_object);
    printf("Объект добавлен в контейнер. Всего объектов: %d\n", getCount());
}

template<class T>
void Container<T>::setObject(int index, T* new_object) {
    if (index == getCount()) {
        myStorage.push_back(new_object);
        printf("Объект добавлен в конец контейнера\n");
    }
    else if (index >= 0 && index < getCount()) {
        delete myStorage[index];
        myStorage[index] = new_object;
        printf("Объект заменен по индексу: %d\n", index);
    } else {
        printf("Попытка установить объект по несуществующему индексу: %d\n", index);
    }
}

template<class T>
void Container<T>::removeAt(int index) {
    if (index < 0 || index >= getCount()) {
        printf("Попытка удаления по несуществующему индексу: %d\n", index);
        return;
    }

    delete myStorage[index];
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
std::vector<T*> Container<T>::getAllObjects() const {
    return myStorage;
}

template<class T>
void Container<T>::clear() {
    for (T* object : myStorage) {
        delete object;
    }
    myStorage.clear();
    printf("Контейнер очищен\n");
}

template<class T>
Container<T>::~Container() {
    for (T* object : myStorage) {
        delete object;
    }
    printf("Контейнер уничтожен\n");
}

template<class T>
std::vector<T*> Container<T>::getSelectedObjects() const {
    std::vector<T*> result;
    for (T* object : myStorage) {
        if (object->isSelected()) {
            result.push_back(object);
        }
    }
    return result;
}

#endif // CONTAINER_H
