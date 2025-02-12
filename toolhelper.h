#ifndef TOOLHELPER_H
#define TOOLHELPER_H

#include <QObject>

class ToolHelper :public QObject
{
    Q_OBJECT
private:
    static ToolHelper* instance;
    ToolHelper();

public:
    static ToolHelper* getInstance(){
        return instance;
    }

    static void buildInstance(){
        if(instance == nullptr){
            instance = new ToolHelper;
        }
    }

    //编辑音乐
    Q_INVOKABLE void editMusic(int coreId);

    //完成编辑音乐
    Q_INVOKABLE void finishEdit(int coreId, QString lrc, QString title = "", QString artist = "", QString alumb = "", QString genre = "", QString year = "");

signals:
    void cppEditMusic(QString, int);
};

#endif // TOOLHELPER_H
