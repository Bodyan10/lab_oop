#ifndef OBSERVER_H
#define OBSERVER_H

class Observer {
 public:
    virtual void onSubjectChanged() = 0;
    virtual ~Observer() = default;
};


#endif // OBSERVER_H
