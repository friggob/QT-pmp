#include "playlist.h"
#include <sys/ioctl.h>
#include <unistd.h>
#include <QFileInfo>
#include <QMimeDatabase>
#include <QDebug>

PlayList::PlayList()
{
	randomize = false;
	pIndex = 0;
}

void PlayList::shuffleList(){
	QStringList t;
	QStringList t2;
	t = pList;

	qsrand(static_cast<uint>(time(nullptr)));

	while(t.count() > 0){
		int i;
		i = qrand() % t.count();
		t2.append(t[i]);
		t.removeAt(i);
	}
	pList = t2;
}

void PlayList::setRandomize(bool tf){
	PlayList::randomize = tf;
}

void PlayList::setIndex(int idx){
	PlayList::pIndex = idx;
}

QString PlayList::getEntry(){

	if(pIndex >= getMaxIndex())
		pIndex = getMaxIndex();

	return PlayList::pList[pIndex];
}

QStringList PlayList::printList(){
	QStringList ls;
	int iLen;
	int tw;
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	tw = w.ws_col;
	iLen = QString::number(pList.count()).length();

	foreach(QString x,pList){
		QString num;
		QString l;
		num = QString::number(pList.indexOf(x));
		if(pList.indexOf(x) == pIndex){
			l += "*";
		}else{
			l += " ";
		}
		l += num.rightJustified(iLen,' ') +
				 " : " +
				 x.leftJustified(tw-10,' ',true);

		ls << l;
	}
	return ls;
}

int PlayList::getIndex(){
	return pIndex;
}

int PlayList::getMaxIndex(){
	return pList.count()-1;
}

void PlayList::removeEntry(){
	pList.removeOne(PlayList::getEntry());
	pIndex--;
}

void PlayList::incrementIndex(){
	pIndex++;
}

void PlayList::decrementIndex(){
	pIndex--;
	if(pIndex < -1)
		pIndex = -1;
}

QStringList PlayList::exportList(){
	return pList;
}

bool PlayList::savePlaylist(QFile* f,bool si = true)
{
	if(!f->open(QIODevice::WriteOnly)){
		qDebug() << "Could not open file" << f->fileName();
		return false;
	}

	if(!f->isWritable()){
		qDebug() << f->fileName() << "is not writeable!";
		return false;
	}

	foreach(QString x, pList){
		QString num;
		num = QString::number(pList.indexOf(x));
		if(pList.indexOf(x) == pIndex && si && (pIndex != getMaxIndex()))
			f->write("*");
		f->write(x.toLocal8Bit()+"\n");
	}
	f->flush();
	f->close();
	return true;
}

int PlayList::createPlayList(QStringList l){
	QMimeType mt;
	QMimeDatabase md;

	foreach(QString p,l){
		QFileInfo fi(p);
		if(!fi.exists()){
			continue;
		}
		mt = md.mimeTypeForFile(fi.filePath(), QMimeDatabase::MatchContent);
		if(!pList.contains(p) &&
			 ( mt.name().startsWith("video/") ||
				 mt.name() == "application/x-matroska" ||
				 mt.name() == "application/vnd.ms-asf" ||
				 mt.name() == "application/x-riff" ||
				 mt.name() == "application/octet-stream" ||
				 mt.name().startsWith("audio/") )){

			PlayList::pList.append(p);

		}	else if(mt.inherits("text/plain") && !fi.fileName().endsWith(".sett")){
			//qStdout() << fi.fileName() << " is a text file, assuming playlist!" << endl;
			QFile fd;
			fd.setFileName(p);
			fd.open(QIODevice::ReadOnly);
			QTextStream ts(&fd);
			QStringList sl;

			while(!ts.atEnd()){
				QString ls = ts.readLine();
				QFileInfo fi2(ls);

				if(ls.startsWith("*")){
					ls.remove(0,1);
					PlayList::settFile = ls;
				}
				if(!l.contains(ls) &&
					 !l.contains(fi2.absoluteFilePath()) &&
					 !l.contains(fi2.filePath())){
					sl.append(ls);
				}else{
					qDebug() << "Found duplicate path!" << ls;
				}
			}

			fd.close();
			createPlayList(sl);
		}else if(mt.inherits("text/plain") && fi.fileName().endsWith(".sett")){
			PlayList::settFile = fi.fileName();
			PlayList::settFile.replace(QString(".sett"),"");

			qDebug() << "File with .sett file:" << settFile;
		}else{
			if(!mt.inherits("inode/directory"))
				qDebug() << p << ":" << mt.name();
		}
	} // foreach(QString p, l)

	if(randomize){
		shuffleList();
	}

	if(settFile.count() > 0){
		QRegExp r(settFile);
		r.setPatternSyntax(QRegExp::FixedString);
		if((pIndex = pList.indexOf(r)) == -1){
			pIndex = 0;
		}
		settFile = "";
	}

	return pList.count();
}
