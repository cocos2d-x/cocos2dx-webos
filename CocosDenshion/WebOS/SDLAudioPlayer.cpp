/*
 * SDLAudioPlayer.cpp
 *
 *  Created on: Jun 07, 2012
 *      Author: Glikeme
 */

#include "SDLAudioPlayer.h"
#include <stdio.h>
#include "stdlib.h"
#include "assert.h"
#include "string.h"


namespace CocosDenshion {

SDLAudioPlayer* SDLAudioPlayer::sharedPlayer() {
	static SDLAudioPlayer s_SharedPlayer;
	return &s_SharedPlayer;
}


SDLAudioPlayer::SDLAudioPlayer() //:
		//pMusic(0), pBGMChannel(0), iSoundChannelCount(0) 
        {
	init();
}

void SDLAudioPlayer::init() {
	//init
}

void SDLAudioPlayer::close() {
	
}

SDLAudioPlayer::~SDLAudioPlayer() {
	
}

// BGM
void SDLAudioPlayer::preloadBackgroundMusic(const char* pszFilePath) {
}

void SDLAudioPlayer::playBackgroundMusic(const char* pszFilePath, bool bLoop) {
}

void SDLAudioPlayer::stopBackgroundMusic(bool bReleaseData) {
}

void SDLAudioPlayer::pauseBackgroundMusic() {
}

void SDLAudioPlayer::resumeBackgroundMusic() {
}

void SDLAudioPlayer::rewindBackgroundMusic() {
}

bool SDLAudioPlayer::willPlayBackgroundMusic() {
	 return false; //do it according to win
}

bool SDLAudioPlayer::isBackgroundMusicPlaying() {
	bool bPlaying = false;
	return bPlaying;

}

float SDLAudioPlayer::getBackgroundMusicVolume() {
	float fVolumn = 1.0f;
	return fVolumn;
}

void SDLAudioPlayer::setBackgroundMusicVolume(float volume) {
}
//~BGM

// for sound effects
float SDLAudioPlayer::getEffectsVolume() {
	float fVolumn = 1.0f;
    return fVolumn;
}

void SDLAudioPlayer::setEffectsVolume(float volume) {
}

unsigned int SDLAudioPlayer::playEffect(const char* pszFilePath, bool bLoop) {
	return 0;
}

void SDLAudioPlayer::stopEffect(unsigned int nSoundId) {
}

void SDLAudioPlayer::pauseEffect(unsigned int uSoundId) {
}

void SDLAudioPlayer::pauseAllEffects() {
}

void SDLAudioPlayer::resumeEffect(unsigned int uSoundId) {
}

void SDLAudioPlayer::resumeAllEffects() {
}

void SDLAudioPlayer::stopAllEffects() {
}

void SDLAudioPlayer::preloadEffect(const char* pszFilePath) {
}

void SDLAudioPlayer::unloadEffect(const char* pszFilePath) {
}

//~for sound effects

} /* namespace CocosDenshion */
