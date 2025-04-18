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

    static void freeInstance(){
        if(instance != nullptr){
            delete instance;
        }
    }


    // 替换文件
    void replaceFile(QString inUrl, QString outUrl);

    QString getFirstKey(QString s);

    //读文件信息
    QString readFileText(QString url);

    //写文件信息
    Q_INVOKABLE bool writeFileText(QString url, QString data);

    // 时间戳转文本
    Q_INVOKABLE QString timeToString(long long time);

    //打开文件
    void deskOpenFile(QString url, bool local);

    //剪切板
    void copyString(QString data);

    // 得到 cout 个 大于 min 小于 max 不重复的随机数
    QList<int> getRandNumber(int min, int max, int count);

    QList<QStringList> getChineseToPinyinJson();

    void sendMessage(QString msg, int type);

    // 重命名文件
    bool renameFile(QString oldUrl, QString newUrl);

    QString getBaseUrl(QString url);
    QString getFileName(QString url);
    QString getParentDir(QString url);

signals:
    void message(QString msg, int type);
};

#endif // BASE_H
