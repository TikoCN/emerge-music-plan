#ifndef LRCDATA_H
#define LRCDATA_H

#include <QObject>

class LrcData : public QObject
{
    Q_OBJECT
public:
    int id;
    int line;//字体行数
    QString text;
    qint64 startTime;
    qint64 endTime;
    double pos;//播放进度
    bool isPlay;

public:
    LrcData();

    QString getText() const;
    void setText(const QString &newText);

    bool getIsPlay() const;
    void setIsPlay(bool newIsPlay);

    double getPos() const;
    void setPos(double newPos);

    int getId() const;
    void setId(int newId);

    int getLine() const;
    void setLine(int newLine);

signals:
    void textChanged();

    void isPlayChanged();

    void idChanged();

    void posChanged();

    void lineChanged();

private:
    Q_PROPERTY(QString text READ getText WRITE setText NOTIFY textChanged FINAL)
    Q_PROPERTY(bool isPlay READ getIsPlay WRITE setIsPlay NOTIFY isPlayChanged FINAL)
    Q_PROPERTY(double pos READ getPos WRITE setPos NOTIFY posChanged FINAL)
    Q_PROPERTY(int id READ getId WRITE setId NOTIFY idChanged FINAL)
    Q_PROPERTY(int line READ getLine WRITE setLine NOTIFY lineChanged FINAL)
};

#endif // LRCDATA_H
