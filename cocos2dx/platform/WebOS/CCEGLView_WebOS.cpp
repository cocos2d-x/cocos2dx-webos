/****************************************************************************
Copyright (c) 2010 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "CCEGLView.h"

#include "gles/gl.h"
#include "SDL.h"
#include "PDL.h"

#include "CCSet.h"
#include "ccMacros.h"
#include "CCDirector.h"
#include "CCTouch.h"
#include "CCTouchDispatcher.h"
#include "CCIMEDispatcher.h"
#include "CCKeypadDispatcher.h"
#include "CCApplication.h"

namespace   cocos2d {


extern "C" {
	GL_API int GL_APIENTRY _dgles_load_library(void *, void *(*)(void *, const char *));
}
//////////////////////////////////////////////////////////////////////////
// impliment CCEGL
//////////////////////////////////////////////////////////////////////////
static void *proc_loader(void *h, const char *name)
{
    (void) h;
    return SDL_GL_GetProcAddress(name);
}

class CCEGL
{
public:
	~CCEGL() 
	{
		
	}

	static CCEGL * create(CCEGLView * pWindow)
	{
		CCEGL * pEGL = new CCEGL;
		do 
		{
			SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE | SDL_INIT_JOYSTICK );
			atexit(SDL_Quit);
    
			// start the PDL library
			PDL_Init(0);
			atexit(PDL_Quit);
    
			// Tell it to use OpenGL version 2.0
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,1 );

			// Set the video mode to full screen with OpenGL-ES support
			// use zero for width/height to use maximum resolution
			pEGL->Surface = SDL_SetVideoMode(0,0, 0, SDL_OPENGL);
		} while (0);

		return pEGL;
	}

	void resizeSurface()
	{

	}
	void Test_Display(void )
{
	// Populate star list
	static float angle = angle+10.0f;
	static float y_trans= y_trans + 5.0f;
	if((y_trans+480) >= 800.0f)
		y_trans = 0.0f;
	float linetry[] = {  0.0f , 0.0f,
		                 480.0f,0.0f,
						 0.0f,800.0f,
						 480.0f,800.0f,
						 
						 320.0f,480.0f,
						 320.0f,0.0f

	};
	float imagetry[] = {  0.0f , 0.0f,
		                 1.0f,0.0f,
						 1.0f,1.0f,
						 
						 0.0f,1.0f,
						 1.0f,1.0f,
						 1.0f,0.0f

	};
	//glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
	glRotatef(angle,0.0f,0.0f,1.0f);
	//glTranslatef(0.0f,y_trans,0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, linetry);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, imagetry);
	glPointSize(20);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glFlush();
	//glFinish();
}
	void swapBuffers()
	{
//         CCLog("Into  swapBuffers \n"); 
		 // Test_Display();
		 SDL_GL_SwapBuffers();
	}
private:
	CCEGL() 
	{ }
	SDL_Surface *Surface;               // Screen surface to retrieve width/height information

};

//////////////////////////////////////////////////////////////////////////
// impliment CCEGLView
//////////////////////////////////////////////////////////////////////////
static CCEGLView * s_pMainWindow;

CCEGLView::CCEGLView()
: m_bCaptured(false)
, m_bOrientationReverted(false)
, m_bOrientationInitVertical(false)
, m_pDelegate(NULL)
, m_pEGL(NULL)
, m_eInitOrientation(CCDeviceOrientationPortrait)
, m_fScreenScaleFactor(1.0f)
{
    m_pTouch    = new CCTouch;
    m_pSet      = new CCSet;
    m_sSizeInPoint.width = m_sSizeInPoint.height = 0;
	memset(&m_rcViewPort,0x0,sizeof(CCRect));
	
}

CCEGLView::~CCEGLView()
{
}

bool CCEGLView::Create(const char* pTitle, int w, int h)
{
	bool bRet = false;
	
	PDL_ScreenMetrics outMetrics;
	PDL_GetScreenMetrics(&outMetrics);
	m_sSizeInPixel.width = outMetrics.horizontalPixels; 
	m_sSizeInPixel.height = outMetrics.verticalPixels;
	
	do 
	{        
		m_eInitOrientation = CCDirector::sharedDirector()->getDeviceOrientation();
        m_bOrientationInitVertical = (CCDeviceOrientationPortrait == m_eInitOrientation
            || kCCDeviceOrientationPortraitUpsideDown == m_eInitOrientation) ? true : false;
        m_sSizeInPoint.width = w;
        m_sSizeInPoint.height = h;    
        /* WebOS doesnt support resizing the window */ 
		resize(w, h);
        
		// init egl
		CCLog("---CALLING CCEGL::create \n ");
		m_pEGL = CCEGL::create(this);
		CCLog("----CALLING CCEGL::create (DONE) \n ");        
		if (! m_pEGL)
		{
			break;
		}

		s_pMainWindow = this;
		bRet = true;
	} while (0);

	// calculate the factor and the rect of viewport	
	m_fScreenScaleFactor =  MIN((float)m_sSizeInPixel.width / m_sSizeInPoint.width,
		                        (float)m_sSizeInPixel.height / m_sSizeInPoint.height);

	int viewPortW = (int)(m_sSizeInPoint.width * m_fScreenScaleFactor);
	int viewPortH = (int)(m_sSizeInPoint.height * m_fScreenScaleFactor);

	m_rcViewPort.origin.x = (m_sSizeInPixel.width - viewPortW) / 2;
	m_rcViewPort.origin.y = (m_sSizeInPixel.height - viewPortH) / 2;
	m_rcViewPort.size.width = viewPortW;
	m_rcViewPort.size.height = viewPortH;
	
	return bRet;
}

long CCEGLView::WindowProc(SDL_EventType message, SDL_Event& Event)
{
    static int xLast = 0x0;
	static int yLast = 0x0;	

    switch(message){
		case SDL_MOUSEBUTTONDOWN:
			 CCLog("--- SDL_MOUSEBUTTONDOWN X= %d,m_rcViewPort.origin.x = %d,y=%d,m_fScreenScaleFactor=%d \n",Event.button.x,m_rcViewPort.origin.x,Event.button.y,m_fScreenScaleFactor);
			 m_pTouch->SetTouchInfo(0, (float)(Event.button.x - m_rcViewPort.origin.x) ,
						(float)(Event.button.y - m_rcViewPort.origin.y) );
			 m_pSet->addObject(m_pTouch);
			 m_pDelegate->touchesBegan(m_pSet, NULL);
			 m_bCaptured = true;
			 break;
		case SDL_MOUSEBUTTONUP:
			if(m_bCaptured == true) {
			m_bCaptured = false;
			CCLog("--- SDL_MOUSEBUTTONUP X= %d,m_rcViewPort.origin.x = %d,y=%d,m_fScreenScaleFactor=%d \n",Event.button.x,m_rcViewPort.origin.x,Event.button.y,m_fScreenScaleFactor);
			m_pTouch->SetTouchInfo(0, (float)(Event.button.x - m_rcViewPort.origin.x) ,
                (float)(Event.button.y - m_rcViewPort.origin.y));
			m_pDelegate->touchesEnded(m_pSet, NULL);
			m_pSet->removeObject(m_pTouch);
			
			}
			break;
		case SDL_MOUSEMOTION:
			//CCLog("--- SDL_MOUSEMOTION(which=%d,state=%d,) X= %d,y=%d,m_fScreenScaleFactor=%d \n",Event.motion.which,Event.motion.state,Event.button.x,Event.button.y,m_fScreenScaleFactor);
			if(Event.motion.state == 1){
			if((xLast==Event.button.x)&&(yLast==Event.button.y))
				break;
			xLast=Event.button.x;
			yLast=Event.button.y;            				
            /* Tracking Mouse Move with Left Button */
			CCLog("--- SDL_MOUSEMOTION(which=%d,state=%d,) X= %d,y=%d,m_fScreenScaleFactor=%d \n",Event.motion.which,Event.motion.state,Event.button.x,Event.button.y,m_fScreenScaleFactor);
			m_pTouch->SetTouchInfo(0, (float)(Event.button.x - m_rcViewPort.origin.x),(float)(Event.button.y - m_rcViewPort.origin.x));
			m_pDelegate->touchesMoved(m_pSet, NULL);
			}
			
			break;		
		default:
			break;
				
    	}
	return 0;
}

CCSize CCEGLView::getSize()
{
	PDL_ScreenMetrics outMetrics;
	
	PDL_Err error = PDL_GetScreenMetrics(&outMetrics);
	if (error == PDL_NOERROR)
    	return CCSize(outMetrics.horizontalPixels,outMetrics.verticalPixels);
    else
    	return CCSize(320,400); //assuming the smallest size, because it will scale up better than down 
}

bool CCEGLView::isOpenGLReady()
{
    return (NULL != m_pEGL);
}

void CCEGLView::release()
{
	s_pMainWindow = NULL;

    CC_SAFE_DELETE(m_pSet);
    CC_SAFE_DELETE(m_pTouch);
    CC_SAFE_DELETE(m_pDelegate);
    CC_SAFE_DELETE(m_pEGL);
    delete this;
}

void CCEGLView::setTouchDelegate(EGLTouchDelegate * pDelegate)
{
    m_pDelegate = pDelegate;
}

void CCEGLView::swapBuffers()
{
    if (m_pEGL)
    {
        m_pEGL->swapBuffers();
    }
}

int CCEGLView::setDeviceOrientation(int eOritation)
{
	// Touchpads are landscape by default, so we want to switch properly in that case
	bool portraitDefault = true;
	
	PDL_ScreenMetrics outMetrics;
	PDL_GetScreenMetrics(&outMetrics);
	if (outMetrics.horizontalPixels > outMetrics.verticalPixels)
		portraitDefault = false;		
	
	if (eOritation == CCDeviceOrientationPortrait || eOritation == CCDeviceOrientationPortraitUpsideDown)
	{
		int width = MIN(m_sSizeInPixel.width, m_sSizeInPixel.height);
		m_sSizeInPixel.height = MAX(m_sSizeInPixel.width, m_sSizeInPixel.height);
		m_sSizeInPixel.width = width;
		width = MIN(m_sSizeInPoint.width, m_sSizeInPoint.height);
		m_sSizeInPoint.height = MAX(m_sSizeInPoint.width, m_sSizeInPoint.height);
		m_sSizeInPoint.width = width;
		resize(m_sSizeInPoint.width, m_sSizeInPoint.height);
		if (portraitDefault)
			CCDirector::sharedDirector()->setDeviceOrientation(CCDeviceOrientationPortrait);
		else
			CCDirector::sharedDirector()->setDeviceOrientation(CCDeviceOrientationLandscapeLeft);	
	}
	else
	{
		int width = MAX(m_sSizeInPixel.width, m_sSizeInPixel.height);
		m_sSizeInPixel.height = MIN(m_sSizeInPixel.width, m_sSizeInPixel.height);
		m_sSizeInPixel.width = width;
		width = MAX(m_sSizeInPoint.width, m_sSizeInPoint.height);
		m_sSizeInPoint.height = MIN(m_sSizeInPoint.width, m_sSizeInPoint.height);
		m_sSizeInPoint.width = width;
		resize(m_sSizeInPoint.width, m_sSizeInPoint.height);
		if (portraitDefault)
			CCDirector::sharedDirector()->setDeviceOrientation(CCDeviceOrientationLandscapeLeft);
		else
			CCDirector::sharedDirector()->setDeviceOrientation(CCDeviceOrientationPortrait);		
	}

	return m_eInitOrientation;
}

void CCEGLView::setViewPortInPoints(float x, float y, float w, float h)
{
    float factor = m_fScreenScaleFactor / CC_CONTENT_SCALE_FACTOR();
}

void CCEGLView::setScissorInPoints(float x, float y, float w, float h)
{
    float factor = m_fScreenScaleFactor / CC_CONTENT_SCALE_FACTOR();
}

void CCEGLView::setIMEKeyboardState(bool /*bOpen*/)
{

}


void CCEGLView::resize(int width, int height)
{

}

void CCEGLView::centerWindow()
{

}

void CCEGLView::setScreenScale(float factor)
{
    CCLog("--- Into setScreenScale ...\n");
//    m_fScreenScaleFactor = factor;
}

bool CCEGLView::canSetContentScaleFactor()
{
    return true;
}

void CCEGLView::setContentScaleFactor(float contentScaleFactor)
{
}

CCEGLView& CCEGLView::sharedOpenGLView()
{
    CC_ASSERT(s_pMainWindow);
    return *s_pMainWindow;
}

}// end of namespace   cocos2d

//NS_CC_END;
