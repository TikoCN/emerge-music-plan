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
    QList<long long> startList;
    QList<long long> endList;
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

    QList<long long> getStartList() const;

    QList<long long> getEndList() const;

    qint64 getStartTime() const;

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
    Q_PROPERTY(QList<long long> startList READ getStartList CONSTANT FINAL)
    Q_PROPERTY(QList<long long> endList READ getEndList CONSTANT FINAL)
    Q_PROPERTY(qint64 startTime READ getStartTime CONSTANT FINAL)
};

#endif // LRCDATA_H
