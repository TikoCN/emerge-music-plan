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

    // 替换文件
    void replaceFile(QString inUrl, QString outUrl);

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

signals:
    void sendMessage(QString msg, int type);
};

#endif // BASE_H
