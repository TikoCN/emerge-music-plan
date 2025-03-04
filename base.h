#ifndef BASE_H
#define BASE_H
#include <QObject>

class Base : public QObject
{
    Q_OBJECT
private:
    static Base* instance;
    explicit Base(){};
public:

    static Base* getInstance(){
        return instance;
    }

    static void buildInstance(){
        if(instance == nullptr){
            instance = new Base;
        }
    }

    //读文件信息
    QString readFileText(QString url);

    //写文件信息
    bool writeFileText(QString url, QString data);

    //打开文件
    void deskOpenFile(QString url, bool local);

    //剪切板
    void copyString(QString data);

signals:
    void sendMessage(QString msg, int type);
};

#endif // BASE_H
