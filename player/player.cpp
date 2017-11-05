#include "player.h"

Player::Player(){
	cacheSize = -1;
	fullScreen = true;
	cIndex = false;
	noSound = false;
	use43 = false;
	use169 = false;
	useStereo = false;
	player = "/usr/bin/mpv";
}

void Player::setCacheSize(int c){
	Player::cacheSize = c;
}

void Player::setSullScreen(bool tf){
	Player::fullScreen = tf;
}

void Player::setcIndex(bool tf){
	Player::cIndex = tf;
}

void Player::setNoSound(bool tf){
	Player::noSound = tf;
}

void Player::setUse43(bool tf){
	Player::use43 = tf;
}

void Player::setUse169(bool tf){
	Player::use169 = tf;
}

void Player::setUseStereo(bool tf){
	Player::useStereo = tf;
}

void Player::createArgList(){
	argList.clear();
	if(fullScreen) argList += "--fs";
	if(cIndex) argList += "--index=recreate";
	if(noSound) argList += "--no-audio";
	if(use43) argList += "--video-aspect=4:3";
	if(use169) argList += "--video-aspect=16:9";
	if(useStereo) argList += "--audio-channels=stereo";
	if(cacheSize != -1){
		QString temp("--cache %1");
		argList += temp.arg(cacheSize);
	}
}

void Player::mpvPlay(QString entry){
	QProcess *proc = new QProcess();
	QStringList a;
	a = argList;

	a += entry;

	proc->setProcessChannelMode(QProcess::ForwardedChannels);

	proc->start(player,a);

	if(!proc->waitForFinished(-1)){
		qDebug() << "Mpv had an error!";
	}

}

void Player::setPlayer(QString path){
	Player::player = path;
}

bool Player::getcIndex(){
	return cIndex;
}

void Player::toggleNosound() {
	noSound = !noSound;
	createArgList();
}
