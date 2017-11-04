#ifndef CLI_H
#define CLI_H

#include "cli_global.h"
#include <../player/player.h>
#include <../playList/playlist.h>
#include <QFileInfo>
#include <QDir>
#include <QtCore>

enum class inStat {NEXT,STOP,AGAIN};

class CLISHARED_EXPORT Cli : public QObject
{
		Q_OBJECT

	public:
		Cli(QObject* parent = 0, Player* player = nullptr, PlayList* list = nullptr);
		inStat processInput(QString in);
		void setDeleteFile(bool t);
		void setMoveFile(bool t);
		void setNosett(bool t);
		void setNodelete(bool t);
		QStringList printHelp();

	signals:
		void helpAvailable();
		void playlistAvailable();
		void statusAvailable(const QString status);

	private:
		Player* mp;
		PlayList* pl;
		bool delFile;
		bool mvFile;
		bool noSett;
		bool noDel;
		void moveEntry(QString d = "sett");
		void createDir(QString d);
		void savePLaylist(QString fn = "__savefile", bool si = true);
		void deletePlaylistFile();
};

#endif // CLI_H
