#ifndef SETTINGATTRIBUTE_H
#define SETTINGATTRIBUTE_H

#define QT_SETTING_PROPERTY(TYPE, PROP, PROP_CAMEL) \
/* 1. 生成Q_PROPERTY声明 */ \
    Q_PROPERTY(TYPE PROP READ get##PROP_CAMEL WRITE set##PROP_CAMEL NOTIFY PROP##Changed FINAL) \
    /* 2. 私有成员变量（命名规范：m_ + 小写属性名） */ \
    private: \
    TYPE m_##PROP; \
    const QString m_##PROP##Key = #PROP; \
    static const bool m_##PROP##_registered; \
    Q_SIGNAL void PROP##Changed(); \
    /* 3. 公共GETTER方法 */ \
    public: \
    TYPE get##PROP_CAMEL() const { return m_##PROP; } \
    /* 4. 带校验的SETTER方法（信号+INI持久化） */ \
    void set##PROP_CAMEL(const TYPE &new##PROP_CAMEL) { \
        if (m_##PROP == new##PROP_CAMEL) return; /* 值未变化则直接返回 */ \
        m_##PROP = new##PROP_CAMEL; \
        Setting::setParameter(m_##PROP##Key, m_##PROP); /* 基础类型直接写入 */ \
        emit PROP##Changed();\
}

#endif // SETTINGATTRIBUTE_H
