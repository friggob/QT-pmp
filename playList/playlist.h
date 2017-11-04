#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "playlist_global.h"
#include <QStringList>
#include <QString>
#include <QTextStream>
#include <QFile>

class PLAYLISTSHARED_EXPORT PlayList
{
	private:
		bool randomize;
		int pIndex;
		QStringList pList;
		void shuffleList();
		QString settFile;

	public:
		PlayList();
		void setRandomize(bool tf);
		void setIndex(int idx);
		int createPlayList(QStringList l);
		QString getEntry();
		QStringList printList(void);
		int getIndex();
		int getMaxIndex();
		void removeEntry();
		void incrementIndex();
		void decrementIndex();
		QStringList exportList();
		bool savePlaylist(QFile *f, bool si);
};

#endif // PLAYLIST_H
