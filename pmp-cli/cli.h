#ifndef CLI_H
#define CLI_H
#include <../playList/playlist.h>
#include <../player/player.h>

#ifndef __QSTDOUT
#define __QSTDOUT
inline QTextStream& qStdout(){
	static QTextStream r{stdout};
	return r;
}
#endif // __QSTDOUT

class cli
{
	private:
	Player* p;
	PlayList* l;
	void printHelp();
	void createSett();
	void deleteSett();
	void createDir(QString d);
	void moveEntry(QString d = "sett");
	void savePlaylist(QString fn = "__savefile", bool si = true);
	void printList();
	bool moveFile;
	bool deleteFile;
	bool noSett;
	bool quiet;
	bool noDelete;

	public:
	cli(Player* player,PlayList* list);
	void run();
	void setDeleteFile(bool t);
	void setMoveFile(bool t);
	void setNoSett(bool t);
	void setQuiet(bool t);
	void setNoDelete(bool t);
};

#endif // CLI_H
