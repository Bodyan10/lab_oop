#ifndef OBSERVABLEMIXIN_H
#define OBSERVABLEMIXIN_H
#include <vector>
#include <Observer.h>

class ObservableMixin {
public:
    void addObserver(Observer* observer);
    void notifyEveryone();
    void removeObserver(Observer* observer);
    std::vector<Observer*> getObservers();
    virtual ~ObservableMixin() = default;

private:
    std::vector<Observer*> observers_;
};

#endif // OBSERVABLEMIXIN_H
