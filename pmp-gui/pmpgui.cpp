#include "pmpgui.h"
#include "ui_pmpgui.h"

pmpGui::pmpGui(QWidget *parent, Cli* c, Player* p, PlayList* l) :
	QMainWindow(parent),
	ui(new Ui::pmpGui)
{
	QLineEdit* le;
	ui->setupUi(this);
	ui->mainToolBar->hide();
	le = ui->cmdBox;
	le->setFocus();
	cli = c;
	mp = p;
	pl = l;
	connect(cli,SIGNAL(helpAvailable()),this,SLOT(printHelp()));
	connect(cli,SIGNAL(playlistAvailable()),this,SLOT(printPlayList()));
	connect(cli,SIGNAL(statusAvailable(QString)),this->statusBar(),SLOT(showMessage(QString)));
}

pmpGui::~pmpGui() {
	delete ui;
}

void pmpGui::setPlayer(Player *p) {
	mp = p;
}

void pmpGui::setPlayList(PlayList *p) {
	pl = p;
}

void pmpGui::setCli(Cli* c) {
	cli = c;
}

void pmpGui::on_actionQuit_triggered() {
	close();
}

void pmpGui::on_cmdBox_returnPressed() {
	QLineEdit* le = ui->cmdBox;
	inStat is;
	is = cli->processInput(le->text());
	switch(is){
		case inStat::STOP:
			close();
			break;
		case inStat::AGAIN:
			break;
		case inStat::NEXT:
			if(pl->getIndex() == 0){
				pl->setIndex(-1);
			}
			pl->incrementIndex();
			mp->mpvPlay(pl->getEntry());
			this->statusBar()->showMessage("Will play index #" + QString::number(pl->getIndex()));
			printPlayList();
			break;
	}

	le->clear();
}

void pmpGui::printHelp() {
	printList(cli->printHelp());
}

void pmpGui::printPlayList(){
	printList(pl->printList());
}

void pmpGui::printList(QStringList ls) {
	QTextBrowser *tb = ui->outBox;
	tb->clear();
	foreach(QString x, ls){
		tb->append(x);
	}
}

void pmpGui::showEvent(QShowEvent* ev) {
	QMainWindow::showEvent(ev);
	if(!ev->spontaneous())
		printPlayList();
}
