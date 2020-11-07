#include "playlist.h"
#include <sys/ioctl.h>
#include <unistd.h>
#include <QFileInfo>
#include <QMimeDatabase>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

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

bool PlayList::savePlaylist(QFile* f, bool si = true, stype st = PlayList::TEXT){
	if(!f->open(QIODevice::WriteOnly)){
		qDebug() << "Could not open file" << f->fileName();
		return false;
	}

	if(!f->isWritable()){
		qDebug() << f->fileName() << "is not writeable!";
		return false;
	}

	if(st == PlayList::TEXT){
		foreach(QString x, pList){
			if(pList.indexOf(x) == pIndex && si && (pIndex != getMaxIndex())){
				f->write("*");
			}
			f->write(x.toLocal8Bit()+"\n");
		}
	}
	if(st == PlayList::JSON){
		QJsonArray ja;
		foreach(QString x, pList){
			QJsonObject jo;
			if(pList.indexOf(x) == pIndex && si && (pIndex != getMaxIndex())){
				jo["active"] = QJsonValue(true);
			} else {
				jo["active"] = QJsonValue(false);
			}
			jo["file"] = QJsonValue(x);
			ja.append(jo);
		}
		f->write("//PMP-PLAYLIST\n");
		f->write(QJsonDocument(ja).toJson(QJsonDocument::Indented));
	}
	f->flush();
	f->close();
	return true;
}

QStringList PlayList::read_json(QString fn){
	QStringList sl;
	QJsonValue js;
	QFile fd;
	fd.setFileName(fn);
	fd.open(QIODevice::ReadOnly);

	QTextStream ts(&fd);

	QString l = ts.readLine(1000);
	if(l == "//PMP-PLAYLIST"){
		// parse_json
	}
	QJsonDocument jsonDoc(QJsonDocument::fromJson(ts.readAll().toLocal8Bit()));

	fd.close();

	foreach(js, jsonDoc.array()){
		QJsonObject jo(js.toObject());
		if(jo["active"].toBool()){
			PlayList::settFile = jo["file"].toString();
		}
		sl.append(jo["file"].toString());
	}

	return sl;
}

QStringList PlayList::read_text(QString fn){
	QStringList sl;
	QFile fd;
	fd.setFileName(fn);
	fd.open(QIODevice::ReadOnly);
	QTextStream ts(&fd);

	while(!ts.atEnd()){
		QString ls = ts.readLine();

		if(ls.startsWith("*")){
			ls.remove(0,1);
			PlayList::settFile = ls;
		}
		sl.append(ls);
	}
	qDebug() << "Added files from textfile!";
	fd.close();
	return sl;
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
			qDebug() << fi.fileName() << " is a text file, assuming playlist!" << endl;
			QStringList sl;
			if((sl = read_json(p)).empty()){
				sl = read_text(p);
			}else{
				qDebug() << fi.fileName() << " is a JSON playlist file!" << endl;
			}
			createPlayList(sl);
		}else if(mt.inherits("text/plain") && fi.fileName().endsWith(".sett")){
			PlayList::settFile = fi.fileName().replace(QString(".sett"),"");
			//PlayList::settFile.replace(QString(".sett"),"");

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
