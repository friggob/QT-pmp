#ifndef PMPGUI_H
#define PMPGUI_H

#include <QMainWindow>
#include <../player/player.h>
#include <../playList/playlist.h>
#include <QRegExp>
#include <QMap>
#include <QShowEvent>

namespace Ui {
	class pmpGui;
}

class pmpGui : public QMainWindow
{
		Q_OBJECT

	public:
		explicit pmpGui(QWidget *parent = 0);
		~pmpGui();
		void setPlayer(Player* p);
		void setPlayList(PlayList* p);

	private slots:
		void on_actionQuit_triggered();
		void on_cmdBox_returnPressed();

	private:
		Ui::pmpGui *ui;
		Player* player;
		PlayList* pList;
		void getInput(QString in);
		void printHelp();
		void printList();

	protected:
		void showEvent(QShowEvent* ev);
};

#endif // PMPGUI_H
