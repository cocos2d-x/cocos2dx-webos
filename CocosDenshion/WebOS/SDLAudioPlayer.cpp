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

static string s_strResourcePath = "./Res/";

static string fullPathFromRelativePath(const char *pszRelativePath)
{
	string strRet="";
	int len = strlen(pszRelativePath);
	if (pszRelativePath == NULL || len <= 0)
	{
		return strRet;
	}

    if (len > 1 && pszRelativePath[0] == '/')
    {
    	strRet = pszRelativePath;
    }
    else
    {
    	strRet = s_strResourcePath;
    	strRet += pszRelativePath;
    }
	return strRet;
}

SDLAudioPlayer* SDLAudioPlayer::sharedPlayer() {
	static SDLAudioPlayer s_SharedPlayer;
	return &s_SharedPlayer;
}


SDLAudioPlayer::SDLAudioPlayer() {
	init();
}

void SDLAudioPlayer::init() {
	SDL_InitSubSystem(SDL_INIT_AUDIO);
	
	int audio_rate = 44100;
	Uint16 audio_format = AUDIO_S16; /* 16-bit stereo */
	int audio_channels = 2;
	int audio_buffers = 4096;
	
	if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers)) 
    	printf("Unable to open audio!\n");
    	
    pMusic = NULL;
    currentMusic = "";
    iEffectsVolume = 64;
}

void SDLAudioPlayer::close() {
	Mix_CloseAudio();
}

SDLAudioPlayer::~SDLAudioPlayer() {
	if (pMusic != NULL)
		stopBackgroundMusic(true);
}

// BGM
void SDLAudioPlayer::preloadBackgroundMusic(const char* pszFilePath) {
	currentMusic = fullPathFromRelativePath(pszFilePath);

	// free old music
	if (pMusic != NULL)
		stopBackgroundMusic(true);
		
	// load new music
	pMusic = Mix_LoadMUS(currentMusic.c_str());
	if (!pMusic)
		printf("Unable to load music file %s: %s", currentMusic.c_str(), Mix_GetError());
}

void SDLAudioPlayer::playBackgroundMusic(const char* pszFilePath, bool bLoop) {
	if (currentMusic != fullPathFromRelativePath(pszFilePath)) 
		preloadBackgroundMusic(pszFilePath);
	
	int loop = bLoop ? -1 : 1;
	if (Mix_PlayMusic(pMusic, loop) == -1) 
   		printf("Error playing %s: %s\n", pszFilePath, Mix_GetError());
}

void SDLAudioPlayer::stopBackgroundMusic(bool bReleaseData) {
	if (Mix_PlayingMusic())
		Mix_HaltMusic();
		
	if (bReleaseData && pMusic != NULL) {
		Mix_FreeMusic(pMusic);
		pMusic = NULL;
		currentMusic = "";
	}
}

void SDLAudioPlayer::pauseBackgroundMusic() {
	if (Mix_PlayingMusic())
		Mix_PauseMusic();
}

void SDLAudioPlayer::resumeBackgroundMusic() {
	Mix_ResumeMusic();
}

void SDLAudioPlayer::rewindBackgroundMusic() {
	Mix_RewindMusic();
}

bool SDLAudioPlayer::willPlayBackgroundMusic() {
	return (pMusic != NULL);
}

bool SDLAudioPlayer::isBackgroundMusicPlaying() {
	return (Mix_PlayingMusic() && !Mix_PausedMusic());
}

float SDLAudioPlayer::getBackgroundMusicVolume() {
	int iVolume = Mix_VolumeMusic(-1);
	float fVolume = iVolume / 128.0;
	return fVolume;
}

void SDLAudioPlayer::setBackgroundMusicVolume(float volume) {
	int iVolume = volume * 128;
	if (iVolume > 128) 
		iVolume = 128;
	else if (iVolume < 0)
		iVolume = 0;

	Mix_VolumeMusic(iVolume);
}
//~BGM

// for sound effects
float SDLAudioPlayer::getEffectsVolume() {
	float fVolume = iEffectsVolume / 128.0;

	return fVolume;
}

void SDLAudioPlayer::setEffectsVolume(float volume) {
	iEffectsVolume = volume * 128;
	if (iEffectsVolume > 128) 
		iEffectsVolume = 128;
	else if (iEffectsVolume < 0)
		iEffectsVolume = 0;
		
	Mix_Volume(-1, iEffectsVolume);
}

unsigned int SDLAudioPlayer::playEffect(const char* pszFilePath, bool bLoop) {
	if(mapEffectSound.find(pszFilePath) == mapEffectSound.end())
		preloadEffect(pszFilePath);

	Mix_VolumeChunk( mapEffectSound[pszFilePath], iEffectsVolume);
	int loop = bLoop ? -1 : 1;
	int iRet = Mix_PlayChannel(-1, mapEffectSound[pszFilePath], loop);
	if (iRet == -1) {
		printf("Error playing %s: %s\n", pszFilePath, Mix_GetError());
		return 0;
	} else {
		return iRet;
	}
}

void SDLAudioPlayer::stopEffect(unsigned int nSoundId) {
	Mix_HaltChannel(nSoundId);
}

void SDLAudioPlayer::pauseEffect(unsigned int uSoundId) {
	Mix_Pause(uSoundId);
}

void SDLAudioPlayer::pauseAllEffects() {
	Mix_Pause(-1);
}

void SDLAudioPlayer::resumeEffect(unsigned int uSoundId) {
	Mix_Resume(uSoundId);
}

void SDLAudioPlayer::resumeAllEffects() {
	Mix_Resume(-1);
}

void SDLAudioPlayer::stopAllEffects() {
	Mix_HaltChannel(-1);
}

void SDLAudioPlayer::preloadEffect(const char* pszFilePath) {
	string filePath = fullPathFromRelativePath(pszFilePath);

	mapEffectSound[pszFilePath] = Mix_LoadWAV(filePath.c_str());
	if (!mapEffectSound[pszFilePath])
		printf("Unable to load sound file %s: %s", currentMusic.c_str(), Mix_GetError());
}

void SDLAudioPlayer::unloadEffect(const char* pszFilePath) {
	if(mapEffectSound.find(pszFilePath) != mapEffectSound.end()) {
		Mix_FreeChunk(mapEffectSound[pszFilePath]);
		mapEffectSound.erase(pszFilePath);
	}
}

//~for sound effects

} /* namespace CocosDenshion */
