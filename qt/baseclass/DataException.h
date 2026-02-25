//
// Created by Administrator on 2026/1/26.
//

#ifndef MUSICPLAY_DATAEXCEPTION_H
#define MUSICPLAY_DATAEXCEPTION_H
#include <QString>
#include <QException>
#include <utility>
#include <QThread>

class DataException : public QException {
public:
    explicit DataException(QString  errorMsg)
    :m_errorMsg(std::move(errorMsg)){

    }

    void raise() const override {
        throw *this; // 抛出当前异常的拷贝
    }

    [[nodiscard]] DataException *clone() const override {
        return new DataException(m_errorMsg); // 创建异常拷贝
    }

    // 获取错误信息
    [[nodiscard]] QString errorMessage() const {
        return m_errorMsg;
    }

private:
    QString m_errorMsg; // 存储错误信息
};
#endif //MUSICPLAY_DATAEXCEPTION_H