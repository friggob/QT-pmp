#include "inputloop.h"
#include <readline/readline.h>
#include <readline/history.h>

inputLoop::inputLoop(QObject *parent, Cli *c, Player *p, PlayList *l) :
	QObject(parent)
{
	mp = p;
	cli = c;
	pl = l;
	connect(cli,SIGNAL(helpAvailable()),this,SLOT(printHelp()));
	connect(cli,SIGNAL(playlistAvailable()),this,SLOT(printPlaylist()));
	connect(cli,SIGNAL(statusAvailable(QString)),this,SLOT(printStatus(QString)));
}

void inputLoop::run() {
	char* buf;
	char ps1[] = "Command? ";
	HIST_ENTRY *he;
	inStat is;

	if(pl->getIndex() > 0)
		pl->incrementIndex();
	printStatus("Playing index #" + QString::number(pl->getIndex()));
	mp->mpvPlay(pl->getEntry());

	while(quiet && (pl->getIndex() < pl->getMaxIndex())){
		pl->incrementIndex();
		mp->mpvPlay(pl->getEntry());
	}

	while(!quiet && ((buf = readline(ps1)) != NULL) &&
				(is = cli->processInput(QString::fromLocal8Bit(buf))) != inStat::STOP) {
		if(buf[0] != 0){
			if((he = previous_history()) == NULL ||
				 (QString::fromLocal8Bit(he->line) !=  QString::fromLocal8Bit(buf)))
			{}
			add_history(buf);
		}
		if(is == inStat::NEXT){
			pl->incrementIndex();
			if(pl->getIndex() > pl->getMaxIndex()){
				printStatus("No more files to play! Bye bye!");
				break;
			}

			printStatus("Playing index #" + QString::number(pl->getIndex()));
			mp->mpvPlay(pl->getEntry());
		}
	}
}

void inputLoop::setQuiet(bool t) {
	quiet = t;
}

void inputLoop::printList(QStringList l) {
	foreach(QString ll,l){
		qStdout() << ll << endl;
	}
}

void inputLoop::printStatus(const QString s) {
	qStdout() << s << endl;
	qStdout().flush();
}

void inputLoop::printHelp() {
	printList(cli->printHelp());
}

void inputLoop::printPlaylist() {
	printList(pl->printList());
}
