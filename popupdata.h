#ifndef POPUPDATA_H
#define POPUPDATA_H
#include <QObject>

class PopupData : public QObject
{
    Q_OBJECT
private:
    static PopupData* instance;
    PopupData();
public:
    static PopupData* getInstance(){
        return instance;
    }

    static void buildInstance(){
        if(instance == nullptr){
            instance = new PopupData;
        }
    }

    //发送对话信息
    void message(QString msg);

signals:
    //发送对话信息
    void cppMessage(QString);
};

#endif // POPUPDATA_H
