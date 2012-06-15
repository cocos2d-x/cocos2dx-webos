#include "CCApplication.h"
#include "CCAccelerometer.h"

#include "CCDirector.h"
#include "SDL.h"
#include "PDL.h"

/**
@brief	This function change the PVRFrame show/hide setting in register.
@param  bEnable If true show the PVRFrame window, otherwise hide.
*/
static void PVRFrameEnableControlWindow(bool bEnable);

NS_CC_BEGIN;

// sharedApplication pointer
CCApplication * CCApplication::sm_pSharedApplication = 0;

CCApplication::CCApplication()
{
    CC_ASSERT(! sm_pSharedApplication);
    sm_pSharedApplication = this;
}

CCApplication::~CCApplication()
{
    CC_ASSERT(this == sm_pSharedApplication);
    sm_pSharedApplication = NULL;
}

int CCApplication::run()
{
    PVRFrameEnableControlWindow(false);
	CCLog("--- INTO CCApplication::run \n");

    // Main message loop:
    
    // Initialize instance and cocos2d.
    if (! initInstance() || ! applicationDidFinishLaunching())
    {
        return 0;
    }
    
    SDL_Joystick *joystick = SDL_JoystickOpen(0);
	
    CCEGLView& mainWnd = CCEGLView::sharedOpenGLView();
    mainWnd.centerWindow();
  	CCLog(" --- BEFORE EVENT LOOP\n");
	SDL_Event Event;
	while (1) {
		bool gotEvent;
        
        // without the inner while loop, input performance gets really laggy    	        
		while ( SDL_PollEvent(&Event) ) {  
//		CCLog("--- GOT NEW EVENT .... \n");
		 	switch (Event.type) {
                case SDL_KEYDOWN:
                    switch (Event.key.keysym.sym) {
                        case PDLK_GESTURE_BACK: /* also maps to ESC */
                            if (PDL_GetPDKVersion() >= 200) {
                                // standard behavior is to minimize to a card when you perform a back
                                // gesture at the top level of the app
                                PDL_Minimize();
                            }
                            break;

                        default:
                            break;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
//					CCLog(" --- GOT NEW EVENT (SDL_MOUSEBUTTONDOWN).... \n");
					mainWnd.WindowProc(SDL_MOUSEBUTTONDOWN,Event);
					break;
				case SDL_MOUSEBUTTONUP:
//					CCLog("--- GOT NEW EVENT (SDL_MOUSEBUTTONUP).... \n");
					mainWnd.WindowProc(SDL_MOUSEBUTTONUP,Event);
					break;
				case SDL_MOUSEMOTION:
					mainWnd.WindowProc(SDL_MOUSEMOTION,Event);
					break;
				case SDL_QUIT:
                    // We exit anytime we get a request to quit the app
                    // all shutdown code is registered via atexit() so this is clean.
                    exit(0);
                    break;
                case SDL_JOYAXISMOTION:
                	CCAccelerometer::sharedAccelerometer()->update(joystick, time(NULL));
                	break;
                case SDL_ACTIVEEVENT:
                	if (Event.active.gain)
                		CCDirector::sharedDirector()->resume();
                	else
                		CCDirector::sharedDirector()->pause();
                	break;
                default:
                    break;
            }
        }
		// Get current time tick.
        // If it's the time to draw next frame, draw it, else sleep a while.
        CCDirector::sharedDirector()->mainLoop();
     }

    return (int) 0;
}

void CCApplication::setAnimationInterval(double interval)
{
    //m_nAnimationInterval.QuadPart = interval; //(LONGLONG)(interval * nFreq.QuadPart);
}

CCApplication::Orientation CCApplication::setOrientation(Orientation orientation)
{
	// Touchpads are landscape by default, so we want to switch properly in that case
	bool portraitDefault = true;
	
	PDL_ScreenMetrics outMetrics;
	PDL_GetScreenMetrics(&outMetrics);
	if (outMetrics.horizontalPixels > outMetrics.verticalPixels)
		portraitDefault = false;
		
	if (orientation == kOrientationPortrait) {
		if (!portraitDefault)
			orientation = kOrientationLandscapeLeft;
	} else if (orientation == kOrientationPortraitUpsideDown) {
		if (!portraitDefault)
			orientation = kOrientationLandscapeRight;	
	} else if (orientation == kOrientationLandscapeLeft){ 
		if (!portraitDefault)
			orientation = kOrientationPortrait;
	} else {
		if (!portraitDefault)
			orientation = kOrientationPortraitUpsideDown;	
	}	
	
	return orientation;
}

void CCApplication::statusBarFrame(CCRect * rect)
{
    if (rect)
    {
        // Windows doesn't have status bar.
        *rect = CCRectMake(0, 0, 0, 0);
    }
}

//////////////////////////////////////////////////////////////////////////
// static member function
//////////////////////////////////////////////////////////////////////////
CCApplication& CCApplication::sharedApplication()
{
    CC_ASSERT(sm_pSharedApplication);
    return *sm_pSharedApplication;
}

ccLanguageType CCApplication::getCurrentLanguage()
{
    ccLanguageType ret = kLanguageEnglish;
    return ret;
}

NS_CC_END;

//////////////////////////////////////////////////////////////////////////
// Local function
//////////////////////////////////////////////////////////////////////////
static void PVRFrameEnableControlWindow(bool bEnable)
{
    
}
