#ifndef PLAYER_H
#define PLAYER_H

#include "player_global.h"
#include <QDebug>
#include <QProcess>
#include <QStringList>

class PLAYERSHARED_EXPORT Player{
	private:
	int cacheSize;
	bool fullScreen;
	bool cIndex;
	bool noSound;
	bool use43;
	bool useStereo;
	bool use169;
	QString player;
	QStringList argList;

	public:
		Player();
		void setCacheSize(int c);
		void setSullScreen(bool tf);
		void setcIndex(bool tf);
		void setNoSound(bool tf);
		void setUse43(bool tf);
		void setUse169(bool tf);
		void setUseStereo(bool tf);
		void createArgList(void);
		void mpvPlay(QString entry);
		void setPlayer(QString path);
		bool getcIndex();
};

#endif // PLAYER_H
