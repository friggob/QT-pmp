#include "cli.h"


Cli::Cli(QObject *parent, Player* player, PlayList* list) :
	QObject(parent)
{
	pl = list;
	mp = player;
	delFile = false;
	mvFile = false;
	noSett = false;
	noDel = false;
}

inStat Cli::processInput(QString in){
	QString dest = "";
	QRegExp re("\\d+");
	bool move = false;
	inStat ex = inStat::NEXT;

	if(mvFile)
		dest = "sett";
	if(delFile)
		dest = ".delete";

	switch(in[0].toLatin1()){
		case 'x':
			if(!noSett){
				deletePlaylistFile();
				noSett = true;
			}
			[[fallthrough]];
		case 'q':
			if(delFile || mvFile)
				moveEntry(dest);
			ex = inStat::STOP;
			break;
		case 'g':
		case 'd':
		case 'm':
			if(in[0] == 'd')
				dest = ".delete";
			if(in[0] == 'g')
				dest = "_gg";
			if(in[0] == 'm')
				dest = "sett";
			if(in.left(2) == "m " && in.count() > 2)
				dest = in.mid(2).trimmed();
			move = true;
			if(in[1] == 'q'){
				moveEntry(dest);
				ex = inStat::STOP;
			}
			break;
		case 'h':
		case '?':
			emit helpAvailable();
			ex = inStat::AGAIN;
			break;
		case 'l':
			emit playlistAvailable();
			ex = inStat::AGAIN;
			break;
		case 'a':
		case 'y':
			if(!noDel){
				QFile df(pl->getEntry());
				if(df.exists()){
					if(df.remove()){
						QString s = "Deleted file: " + pl->getEntry();
						//qDebug() << s;
						emit statusAvailable(s);
						pl->removeEntry();
					}
				}
			}
			break;
		case 'r':
			pl->decrementIndex();
			break;
		case 's': {
			QString sfile = "";

			sfile = "__savefile";
			bool si = true;
			if(in.left(2) == "s " && in.count() > 2){
				sfile = in.mid(2).trimmed();
				si = false;
			}
			savePLaylist(sfile,si);
			ex = inStat::AGAIN;
			if(in[1] == 'q')
				ex = inStat::STOP;
			break;
		}
		case 'n':
			mp->toggleNosound();
			ex = inStat::AGAIN;
			break;
	}
	if(re.exactMatch(in)){
		if(in.toInt() <= pl->getMaxIndex()){
			emit statusAvailable("Setting index to "+in);
			pl->setIndex(in.toInt() - 1);
		}
		emit playlistAvailable();
		ex = inStat::AGAIN;
	}

	if(move || ((mvFile || delFile) && in == ""))
		moveEntry(dest);

	if(!noSett)
		savePLaylist();

	return ex;
}

void Cli::setDeleteFile(bool t){
	delFile = t;
}

void Cli::setMoveFile(bool t){
	mvFile = t;
}

void Cli::setNosett(bool t){
	noSett = t;
}

void Cli::setNodelete(bool t){
	noDel = t;
}

QStringList Cli::printHelp() {
	QMap<QString,QString> m;
	QStringList sl;

	m.insert("?|h|help",
					 "This help");
	m.insert("x",
					 "Quit and delete .sett file");
	m.insert("q",
					 "Quit");
	m.insert("a|y",
					 "Delete file");
	m.insert("d",
					 "Move file to dir ./.delete");
	m.insert("dq",
					 "Move file to dir ./.delete and quit");
	m.insert("i",
					 "Toggle forced reindex when playing file");
	m.insert("r",
					 "Replay last file");
	m.insert("l",
					 "Print playlist");
	m.insert("n",
					 "Toggle no sound setting on player");
	m.insert("#",
					 "Play file at index # in playlist");
	m.insert("g",
					 "Move file to dir ./_gg");
	m.insert("gq",
					 "Move file to dir ./_gg and quit");
	m.insert("m",
					 "Move file to dir ./sett");
	m.insert("m <dir>",
					 "Move file into dir named <dir>");
	m.insert("mq",
					 "Move file to dir ./sett and quit");
	m.insert("s",
					 "Save playlist into file ./__savelist");
	m.insert("sq",
					 "Save playlist into file ./__savelist and quit");

	sl << "Command:";
	foreach(QString k, m.keys()) {
		QString l = k.rightJustified(8,' ') + ": " +
								m[k];
		sl << l;
	}
	return sl;
}

void Cli::moveEntry(QString d){
	createDir(d);
	QFile f(pl->getEntry());
	QFileInfo fi(pl->getEntry());
	if(f.rename(d+"/"+fi.fileName())) {
		QString s = "Moved file "+fi.fileName()+" to: "+d+"/";
		//qDebug() << s;
		emit statusAvailable(s);
		pl->removeEntry();
	}
}

void Cli::createDir(QString d){
	QDir dir;

	dir.setPath(d);
	if(!dir.exists()) {
		QDir::current().mkdir(d);
		QString s = dir.path() + " was created";
		//qDebug() << s;
		emit statusAvailable(s);
	}
}

void Cli::savePLaylist(QString fn, bool si){
	QFile fd(fn);

	if(!pl->savePlaylist(&fd,si)){
		QString s = "Could not save playlist!";
		//qDebug() << s;
		emit statusAvailable(s);
	} else {
		emit statusAvailable("Playlist saved to file: "+fn);
	}
}

void Cli::deletePlaylistFile() {
	QFile f("__savefile");

	if(f.exists())
		f.remove();
}
