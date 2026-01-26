#ifndef BASETOOL_H
#define BASETOOL_H

#include "filemanagement.h"
#include "typeconversion.h"
#include "qmlactive.h"

class BaseTool : public QObject {
    Q_OBJECT

public:
    static BaseTool *getInstance() {
        return instance;
    }

    static void buildInstance() {
        if (instance == nullptr) {
            instance = new BaseTool;
        }
    }

    static void freeInstance() {
        delete instance;
    }

    Q_INVOKABLE static void copyString(const QString &data);

    static QString getErrorMsg(const QThread *thread, const QString &errorMsg);

    [[nodiscard]] const TypeConversion *getTypeConversion() const;

    [[nodiscard]] const FileManagement *getFileManagement() const;

    [[nodiscard]] const QmlActive *getQmlActive() const;

private:
    static BaseTool *instance;

    explicit BaseTool();

    const TypeConversion *typeConversion;
    const FileManagement *fileManagement;
    const QmlActive *qmlActive;

    Q_PROPERTY(const FileManagement *fileManagement READ getFileManagement CONSTANT FINAL)
    Q_PROPERTY(const QmlActive *qmlActive READ getQmlActive CONSTANT FINAL)
    Q_PROPERTY(const TypeConversion *typeConversion READ getTypeConversion CONSTANT FINAL)
};

#endif // BASETOOL_H
