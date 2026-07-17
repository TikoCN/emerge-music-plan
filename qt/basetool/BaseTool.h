#ifndef BASETOOL_H
#define BASETOOL_H

#include "FileManagement.h"
#include "TypeConversion.h"
#include "QmlActive.h"

class BaseTool : public QObject {
    Q_OBJECT

public:
    static BaseTool &getInstance() {
        static BaseTool instance;
        return instance;
    }

    Q_INVOKABLE static void copyString(const QString &data);

    [[nodiscard]] const TypeConversion *getTypeConversion() const;

    [[nodiscard]] const FileManagement *getFileManagement() const;

    [[nodiscard]] const QmlActive *getQmlActive() const;

private:
    explicit BaseTool();

    const TypeConversion *typeConversion;
    const FileManagement *fileManagement;
    const QmlActive *     qmlActive;

    Q_PROPERTY(const FileManagement *fileManagement READ getFileManagement CONSTANT FINAL)
    Q_PROPERTY(const QmlActive *qmlActive READ getQmlActive CONSTANT FINAL)
    Q_PROPERTY(const TypeConversion *typeConversion READ getTypeConversion CONSTANT FINAL)
};

#endif // BASETOOL_H
