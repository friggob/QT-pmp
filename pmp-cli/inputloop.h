#ifndef INPUTLOOP_H
#define INPUTLOOP_H
#include <../cli/cli.h>

#ifndef __QSTDOUT
#define __QSTDOUT
inline QTextStream& qStdout(){
	static QTextStream r{stdout};
	return r;
}
#endif // __QSTDOUT

class inputLoop : public QObject {

		Q_OBJECT

	public:
		inputLoop(QObject* parent = 0, Cli* c = nullptr, Player* p = nullptr, PlayList* l = nullptr);
		void setQuiet(bool t);

	public slots:
		void run();

	private:
		Player* mp;
		PlayList* pl;
		Cli* cli;
		bool quiet;
		void printList(QStringList l);

	private slots:
		void printStatus(const QString s);
		void printHelp();
		void printPlaylist();
};

#endif // INPUTLOOP_H
