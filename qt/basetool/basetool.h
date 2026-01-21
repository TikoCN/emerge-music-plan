#ifndef BASETOOL_H
#define BASETOOL_H

#include <QObject>
#include "filemanagement.h"
#include "typeconversion.h"
#include "qmlactive.h"

class BaseTool : public QObject
{
    Q_OBJECT
public:
    static BaseTool* getInstance(){
        return instance;
    }

    static void buildInstance(){
        if(instance == nullptr){
            instance = new BaseTool;
        }
    }

    static void freeInstance(){
        if(instance != nullptr){
            delete instance;
        }
    }

    Q_INVOKABLE void copyString(QString data);

    const TypeConversion *getTypeConversion() const;

    const FileManagement *getFileManagement() const;

    const QmlActive *getQmlActive() const;

private:
    static BaseTool* instance;
    explicit BaseTool();

    const TypeConversion *typeConversion;
    const FileManagement *fileManagement;
    const QmlActive *qmlActive;

    Q_PROPERTY(const FileManagement *fileManagement READ getFileManagement CONSTANT FINAL)
    Q_PROPERTY(const QmlActive *qmlActive READ getQmlActive CONSTANT FINAL)
    Q_PROPERTY(const TypeConversion *typeConversion READ getTypeConversion CONSTANT FINAL)
};

#endif // BASETOOL_H
