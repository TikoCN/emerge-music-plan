#ifndef REFERENCECOUNT_H
#define REFERENCECOUNT_H

class ReferenceCount
{
private:
    int m_cout;
public:
    ReferenceCount();
    void increment();

    bool decrement();

    int getCount() const;
};

#endif // REFERENCECOUNT_H
