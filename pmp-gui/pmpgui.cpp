#include "pmpgui.h"
#include "ui_pmpgui.h"

pmpGui::pmpGui(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::pmpGui)
{
	QLineEdit* le;
	ui->setupUi(this);
	ui->mainToolBar->hide();
	le = ui->cmdBox;
	le->setFocus();
}

pmpGui::~pmpGui()
{
	delete ui;
}

void pmpGui::setPlayer(Player *p)
{
	player = p;
}

void pmpGui::setPlayList(PlayList *p)
{
	pList = p;
}

void pmpGui::on_actionQuit_triggered()
{
	close();
}

void pmpGui::on_cmdBox_returnPressed()
{
	//QTextBrowser* tb = ui->outBox;
	QLineEdit* le = ui->cmdBox;
	getInput(le->text());
	le->clear();
}

void pmpGui::getInput(QString in){
	//QTextBrowser* tb = ui->outBox;
	QRegExp r1("\\d+");
	QRegExp r2("^(m|g|d).*");
	bool play = false;

	if(in.startsWith('l')){
		printList();
	}
	if(in.startsWith('q') || in.startsWith('x')){
		close();
		return;
	}
	if(r1.exactMatch(in)){
		if(in.toInt() <= pList->getMaxIndex()){
			pList->setIndex(in.toInt()-1);
		}
		play = true;
	}
	if(in.startsWith('r')){
		pList->decrementIndex();
		play = true;
	}
	if(in.startsWith('h') || in.startsWith('?')){
		printHelp();
	}
	if(in == "")
		play = true;
	if(play){
		pList->incrementIndex();
		player->mpvPlay(pList->getEntry());
		printList();
	}
}

void pmpGui::printHelp() {
	QMap<QString,QString> m;
	QTextBrowser* tb = ui->outBox;

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

	tb->clear();
	tb->append(" Command:");
	foreach(QString k, m.keys()) {
		tb->append(k.rightJustified(8,' ') + ": " + m[k]);
	}
}

void pmpGui::printList(){
	QTextBrowser *tb = ui->outBox;
	tb->clear();
	foreach(QString x, pList->printList()){
		tb->append(x);
	}
}

void pmpGui::showEvent(QShowEvent* ev){
	//QMainWindow::showEvent(ev);
	if(!ev->spontaneous())
		printList();
}
