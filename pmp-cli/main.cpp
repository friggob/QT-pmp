#include <QCoreApplication>
#include <../player/player.h>
#include <../playList/playlist.h>
#include <cli.h>
#include <QCommandLineParser>
#include <QDebug>

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	QCommandLineParser pa;
	Player p;
	PlayList pl;
	cli* c = new cli(&p,&pl);

	a.setApplicationName("pmp-cli");
	a.setApplicationVersion(APP_VERSION);

	pa.setApplicationDescription("Playlist player using mpv");
	pa.addHelpOption();
	pa.addVersionOption();

	pa.addOptions({
		{"D","Do not delete, even if we say so"},
		{"n","No audio"},
		{"c","Cache size, in kilobytes","cache"},
		{"i","Create index before playing file"},
		{"s","Force 4:3 aspect"},
		{"w","Force 16:9 aspect"},
		{"z","Randomize playlist"},
		{"d","Automatically move file to .delete folder"},
		{"m","Automatically move file to sett folder"},
		{"q","Do not wait for commands between playing files"},
		{"r","Index in playlist to start from","entry"},
		{"t","Force stereo"},
		{"x","Don't create .sett file"}
	});

	pa.process(a);
	const QStringList args = pa.positionalArguments();

	pl.setRandomize(pa.isSet("z"));
	if(pl.createPlayList(args) < 1) {
		qDebug() << "Error, playlist empty!";
		return 1;
	}
	if(pa.isSet("r")) pl.setIndex(pa.value("r").toInt());

	p.setNoSound(pa.isSet("n"));
	p.setcIndex(pa.isSet("i"));
	p.setUse43(pa.isSet("s"));
	p.setUse169(pa.isSet("w"));
	p.setUseStereo(pa.isSet("t"));
	if(pa.isSet("c")) p.setCacheSize(pa.value("c").toInt());
	p.createArgList();

	c->setNoSett(pa.isSet("x"));
	c->setDeleteFile(pa.isSet("d"));
	c->setMoveFile(pa.isSet("m"));
	c->setQuiet(pa.isSet("q"));
	c->setNoDelete(pa.isSet("D"));
	c->run();

	//return a.exec();
}
