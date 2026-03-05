#ifndef CONSTANTPROPERTY_H
#define CONSTANTPROPERTY_H

#define QT_CONSTANT_PROPERTY(TYPE, PROP, PROP_CAMEL) \
/* 1. 生成Q_PROPERTY声明 */ \
    Q_PROPERTY(TYPE PROP READ get##PROP_CAMEL CONSTANT) \
    /* 2. 成员变量*/ \
    TYPE PROP; \
    /* 3. 公共GETTER方法 */ \
    TYPE get##PROP_CAMEL() const { return PROP; }

#endif // CONSTANTPROPERTY_H
