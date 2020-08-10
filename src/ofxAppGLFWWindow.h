
#if defined(JUCE_APP_VERSION)
#else
#include "ofMain.h"
#include "ofGLProgrammableRenderer.h"
//#include "flextGL.h"
#include "GLFW/glfw3.h"
#endif

class ofxAppGLFWWindow : public ofAppBaseWindow {
    std::unique_ptr<ofCoreEvents> coreEvents;
    std::shared_ptr<ofBaseRenderer> currentRenderer;
    int width = 0;
    int height = 0;
    
public:
    
    ofxAppGLFWWindow():coreEvents(new ofCoreEvents){
        
    };
    ~ofxAppGLFWWindow(){};
    
    void setup(const ofWindowSettings & settings) {
    }
    
    void setup(const ofGLFWWindowSettings & settings) {
        
        if(!glfwInit( )){
            ofLogError("ofAppGLFWWindow") << "couldn't init GLFW";
            //return;
        }
        
		/*
        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_RED_BITS, settings.redBits);
        glfwWindowHint(GLFW_GREEN_BITS, settings.greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, settings.blueBits);
        glfwWindowHint(GLFW_ALPHA_BITS, settings.alphaBits);
        glfwWindowHint(GLFW_DEPTH_BITS, settings.depthBits);
        glfwWindowHint(GLFW_STENCIL_BITS, settings.stencilBits);
        glfwWindowHint(GLFW_STEREO, settings.stereo);
        glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
#ifndef TARGET_OSX
        glfwWindowHint(GLFW_AUX_BUFFERS, settings.doubleBuffering?1:0);
#endif
*/
		/*
        glfwWindowHint(GLFW_SAMPLES, settings.numSamples);
        glfwWindowHint(GLFW_RESIZABLE, settings.resizable);
        glfwWindowHint(GLFW_DECORATED, settings.decorated);
		*/
#ifdef TARGET_OPENGLES
        /*
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, settings.glesVersion);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
		*/
        if(settings.glesVersion>=2){
            currentRenderer = std::make_shared<ofGLProgrammableRenderer>(this);
        }else{
            currentRenderer = std::make_shared<ofGLRenderer>(this);
        }
#else
        /*
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, settings.glVersionMajor);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, settings.glVersionMinor);
		*/
        if((settings.glVersionMajor==3 && settings.glVersionMinor>=2) || settings.glVersionMajor>=4){
        //    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        }
        if(settings.glVersionMajor>=3){
       //     glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
            currentRenderer = std::make_shared<ofGLProgrammableRenderer>(this);
        }else{
            currentRenderer = std::make_shared<ofGLRenderer>(this);
        }
#endif
        /*
        GLFWwindow * sharedContext = nullptr;
        if(settings.shareContextWith){
            sharedContext = (GLFWwindow*)settings.shareContextWith->getWindowContext();
        }
        */

		setWindowShape(800, 600);
		//setWindowPosition(0, 0);

#ifndef TARGET_OPENGLES
		glewExperimental = GL_TRUE;
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			/* Problem: glewInit failed, something is seriously wrong. */
			ofLogError("ofAppRunner") << "couldn't init GLEW: " << glewGetErrorString(err);
		}
#endif
        
        ofLogVerbose() << "GL Version:" << glGetString(GL_VERSION);

		while(glGetError() != GL_NO_ERROR) {
			// Process the error
		}

		
        if(currentRenderer->getType()==ofGLProgrammableRenderer::TYPE){
#ifndef TARGET_OPENGLES
            static_cast<ofGLProgrammableRenderer*>(currentRenderer.get())->setup(settings.glVersionMajor,settings.glVersionMinor);
#else
            static_cast<ofGLProgrammableRenderer*>(currentRenderer.get())->setup(settings.glesVersion,0);
#endif
        } else {
            static_cast<ofGLRenderer*>(currentRenderer.get())->setup();
        }
    }
    
    void update() {}
    void draw() { }
    virtual bool getWindowShouldClose(){
        return false;
    }
    virtual void setWindowShouldClose(){
    }
    virtual void close(){
    }
    
    ofCoreEvents & events(){
        return *coreEvents;
    }
    
    shared_ptr<ofBaseRenderer> & renderer(){
        return currentRenderer;
    }
    
    virtual void hideCursor() {}
    virtual void showCursor() {}
    
    virtual void    setWindowPosition(int x, int y) {}
    void    setWindowShape(int w, int h) {
        width = w;
        height = h;
    }
    
    glm::vec2    getWindowPosition() {return glm::vec2(0,0); }
    glm::vec2    getWindowSize(){return glm::vec2(width,height); }
    glm::vec2    getScreenSize(){return glm::vec2(width,height); }
    
    virtual void            setOrientation(ofOrientation orientation){ }
    virtual ofOrientation    getOrientation(){ return OF_ORIENTATION_DEFAULT; }
    virtual bool    doesHWOrientation(){return false;}
    
    //this is used by ofGetWidth and now determines the window width based on orientation
    int        getWidth(){
        return  width;
        
    }
    int        getHeight(){ return height; }
    
    virtual void    setWindowTitle(std::string title){}
    
    virtual ofWindowMode    getWindowMode() {return OF_WINDOW ;}
    
    virtual void    setFullscreen(bool fullscreen){}
    virtual void    toggleFullscreen(){}
    
    virtual void    enableSetupScreen(){}
    virtual void    disableSetupScreen(){}
    
    virtual void    setVerticalSync(bool enabled){};
    virtual void    setClipboardString(const std::string& text) {}
    virtual std::string  getClipboardString() { return ""; }
    
    void makeCurrent(){};
    void swapBuffers() {}
    
    //--------------------------------------------
    void startRender() {
        renderer()->startRender();
    }
    
    //--------------------------------------------
    void finishRender() {
        renderer()->finishRender();
    }
    
    
    virtual void * getWindowContext(){return nullptr;};
    
#if defined(TARGET_LINUX) && !defined(TARGET_RASPBERRY_PI)
    virtual Display* getX11Display(){return nullptr;}
    virtual Window  getX11Window() {return 0;}
#endif
    
#if defined(TARGET_LINUX) && !defined(TARGET_OPENGLES)
    virtual GLXContext getGLXContext(){return 0;}
#endif
    
#if defined(TARGET_LINUX) && defined(TARGET_OPENGLES)
    virtual EGLDisplay getEGLDisplay(){return 0;}
    virtual EGLContext getEGLContext(){return 0;}
    virtual EGLSurface getEGLSurface(){return 0;}
#endif
    
#if defined(TARGET_OSX)
    virtual void * getNSGLContext(){return nullptr;}
    virtual void * getCocoaWindow(){return nullptr;}
#endif
    
#if defined(TARGET_WIN32)
    virtual HGLRC getWGLContext(){return 0;}
    virtual HWND getWin32Window(){return 0;}
#endif
};

#if defined(JUCE_APP_VERSION)
#else
#endif



