#include "pmpgui.h"
#include <QApplication>
#include <QCommandLineParser>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QCommandLineParser pa;
	Player p;
	PlayList pl;
	Cli cli(&a, &p, &pl);
	pmpGui w(0,&cli,&p,&pl);

	a.setApplicationName("pmp-gui");
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
		{"x","Create __savefile"}
	});

	pa.process(a);
	const QStringList args = pa.positionalArguments();

	pl.setRandomize(pa.isSet("z"));
	pl.createPlayList(args);
	if(pa.isSet("r"))
		pl.setIndex(pa.value("r").toInt());

	p.setNoSound(pa.isSet("n"));
	p.setcIndex(pa.isSet("i"));
	p.setUse43(pa.isSet("s"));
	p.setUse169(pa.isSet("w"));
	p.setUseStereo(pa.isSet("t"));
	if(pa.isSet("c"))
		p.setCacheSize(pa.value("c").toInt());
	p.createArgList();

	cli.setNosett(!pa.isSet("x"));
	cli.setNodelete(pa.isSet("D"));
	cli.setMoveFile(pa.isSet("m"));
	cli.setDeleteFile(pa.isSet("d"));

	w.setPlayer(&p);
	w.setPlayList(&pl);
	w.setCli(&cli);

	w.show();

	return a.exec();
}
