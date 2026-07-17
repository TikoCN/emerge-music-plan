//
// Created by changzhi on 2026/2/19.
//

#ifndef MUSICPLAY_LITERALCONSTANT_H
#define MUSICPLAY_LITERALCONSTANT_H
#include <QString>

namespace LiteralConstant {
    namespace Table {
        inline const QString ARTIST         = QStringLiteral("artist");
        inline const QString ARTIST_MUSIC   = QStringLiteral("artist_music");
        inline const QString ALBUM          = QStringLiteral("album");
        inline const QString ALBUM_MUSIC    = QStringLiteral("album_music");
        inline const QString MUSIC          = QStringLiteral("music");
        inline const QString PLAYLIST       = QStringLiteral("playlist");
        inline const QString PLAYLIST_MUSIC = QStringLiteral("playlist_music");
        inline const QString PLAYINGLIST    = QStringLiteral("playinglist");
    }

    namespace Column {
        inline const QString MUSIC_ID       = QStringLiteral("music_id");
        inline const QString TITLE          = QStringLiteral("title");
        inline const QString DURATION       = QStringLiteral("duration");
        inline const QString LEVEL          = QStringLiteral("level");
        inline const QString PLAY_NUMBER    = QStringLiteral("play_number");
        inline const QString LAST_EDIT_TIME = QStringLiteral("last_edit_time");
        inline const QString IS_LOVE        = QStringLiteral("is_love");
        inline const QString NAME_KEY       = QStringLiteral("name_key");
        inline const QString URL            = QStringLiteral("url");

        inline const QString ARTIST_NAME = QStringLiteral("artist_name");
        inline const QString ARTIST_ID   = QStringLiteral("artist_id");

        inline const QString ALBUM_NAME = QStringLiteral("album_name");
        inline const QString ALBUM_ID   = QStringLiteral("album_id");

        inline const QString PLAYLIST_NAME = QStringLiteral("playlist_name");
        inline const QString PLAYLIST_ID   = QStringLiteral("playlist_id");
        inline const QString IS_DIR        = QStringLiteral("is_dir"); // 新增

        inline const QString POSITION = QStringLiteral("position");
        inline const QString SORT     = QStringLiteral("sort");
    }

    inline const auto ASC  = QStringLiteral("ASC");
    inline const auto DESC = QStringLiteral("DESC");

    inline const QString VOLUME             = QStringLiteral("volume");
    inline const QString PLAY_MUSIC_LIST_ID = QStringLiteral("play_music_list_id");
}

// LiteralConstant

#endif //MUSICPLAY_LITERALCONSTANT_H
