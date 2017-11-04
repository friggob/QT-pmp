#ifndef PMPGUI_H
#define PMPGUI_H

#include <QMainWindow>
#include <../player/player.h>
#include <../playList/playlist.h>
#include <../cli/cli.h>
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
		explicit pmpGui(QWidget *parent = 0, Cli *c = nullptr, Player *p = nullptr, PlayList *l = nullptr);
		~pmpGui();
		void setPlayer(Player* p);
		void setPlayList(PlayList* p);
		void setCli(Cli* c);

	private slots:
		void on_actionQuit_triggered();
		void on_cmdBox_returnPressed();
		void printPlayList();
		void printHelp();

	private:
		Ui::pmpGui *ui;
		Player* mp;
		PlayList* pl;
		Cli* cli;
		void printList(QStringList ls);

	protected:
		void showEvent(QShowEvent* ev);
};

#endif // PMPGUI_H
