#include "observablemixin.h"

void ObservableMixin::addObserver(Observer* observer) {
    observers_.push_back(observer);
}

void ObservableMixin::notifyEveryone() {
    for (Observer* observer : observers_) {
        observer->onSubjectChanged();
    }
}

void ObservableMixin::removeObserver(Observer* observer) {
    auto it = std::find(observers_.begin(), observers_.end(), observer);
    if (it != observers_.end()) {
        observers_.erase(it);
    }
}

std::vector<Observer*> ObservableMixin::getObservers() {
    return observers_;
}
