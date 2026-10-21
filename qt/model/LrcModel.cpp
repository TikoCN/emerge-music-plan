#include "LrcModel.h"

namespace {

QVariantList toVariantList(const QVector<QString> &values) {
    QVariantList result;
    result.reserve(values.size());
    for (const QString &value : values) {
        result.append(value);
    }
    return result;
}

QVariantList toVariantList(const QVector<long long> &values) {
    QVariantList result;
    result.reserve(values.size());
    for (const long long value : values) {
        result.append(QVariant::fromValue<qlonglong>(value));
    }
    return result;
}

}

LrcModel::LrcModel(QObject *parent)
    : QAbstractListModel(parent) {
}

int LrcModel::rowCount(const QModelIndex &parent) const {
    return parent.isValid() ? 0 : static_cast<int>(m_lyrics.size());
}

QVariant LrcModel::data(const QModelIndex &index, const int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= m_lyrics.size()) {
        return {};
    }

    const LrcDataPtr &lyric = m_lyrics[index.row()];
    switch (role) {
        case LrcIdRole:
            return lyric->getId();
        case StartTimeRole:
            return lyric->getStartTime();
        case EndTimeRole:
            return lyric->getEndTime();
        case StartListRole:
            return toVariantList(lyric->getStartList());
        case EndListRole:
            return toVariantList(lyric->getEndList());
        case TextListRole:
            return toVariantList(lyric->getTextList());
        case HelpTextListRole:
            return toVariantList(lyric->getHelpTextList());
        default:
            return {};
    }
}

QHash<int, QByteArray> LrcModel::roleNames() const {
    return {
        {LrcIdRole, "lrcId"},
        {StartTimeRole, "startTime"},
        {EndTimeRole, "endTime"},
        {StartListRole, "startList"},
        {EndListRole, "endList"},
        {TextListRole, "textList"},
        {HelpTextListRole, "helpTextList"}
    };
}

void LrcModel::setLyrics(const QList<LrcDataPtr> &lyrics) {
    beginResetModel();
    m_lyrics = lyrics;
    endResetModel();
}

void LrcModel::clear() {
    if (m_lyrics.isEmpty()) {
        return;
    }
    beginResetModel();
    m_lyrics.clear();
    endResetModel();
}
