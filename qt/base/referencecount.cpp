#include "referencecount.h"

ReferenceCount::ReferenceCount() {
    m_cout = 0;
}

void ReferenceCount::increment()
{
    m_cout++;
}

bool ReferenceCount::decrement()
{
    m_cout--;
    return m_cout == 0;
}

int ReferenceCount::getCount() const
{
    return m_cout;
}
