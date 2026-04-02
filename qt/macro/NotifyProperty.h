#ifndef NotifyProperty_H
#define NotifyProperty_H

#define QT_NOTIFY_PROPERTY(TYPE, PROP, PROP_CAMEL) \
/* 1. 生成Q_PROPERTY声明 */ \
Q_PROPERTY(TYPE PROP READ get##PROP_CAMEL WRITE set##PROP_CAMEL NOTIFY PROP##Changed FINAL) \
/* 2. 私有成员变量（命名规范：m_ + 小写属性名） */ \
private: \
TYPE PROP; \
Q_SIGNAL void PROP##Changed(); \
/* 3. 公共GETTER方法 */ \
public: \
TYPE get##PROP_CAMEL() const { return PROP; } \
/* 4. 带校验的SETTER方法 */ \
void set##PROP_CAMEL(const TYPE &new##PROP_CAMEL) { \
if (PROP == new##PROP_CAMEL) return; /* 值未变化则直接返回 */ \
PROP = new##PROP_CAMEL; \
emit PROP##Changed();\
}

#endif // NotifyProperty_H
