#include "PluginBase.h"

#if defined(JUCE_APP_VERSION)
#else
#include "ofxAppGLFWWindow.h"
#endif

class ofxPluginWithRender : public hotjuice::PluginBase {
protected:
#if defined(JUCE_APP_VERSION)
#else
	ofxAppGLFWWindow window;
#endif
	std::string _log;
	long long _timeStart;

public:
	ofxPluginWithRender() {
		_log = "";
		_timeStart = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}

	virtual void setupRenderer(void* in = nullptr, void* out = nullptr) override {
#if defined(JUCE_APP_VERSION)
#else
		ofGLFWWindowSettings settings;
		settings.setSize(1, 1);
		settings.glVersionMajor = 3;
		settings.glVersionMinor = 2;
		window.setup(settings);
#endif
	}
    
    float getDesktopScale() {
        return desktopScale_;
    }
    
    float desktopScale_;
    
    void setDesktopScale(float desktopScale) override {
        desktopScale_ = desktopScale;
    }

	void setWindowSize(int w, int h) override {
#if defined(JUCE_APP_VERSION)
#else
		ofBaseRenderer& renderer = *(window.renderer());
		if (window.getWidth() != w || window.getHeight() != h) {
			window.setWindowShape(w, h);
		}
		renderer.setupScreenOrtho(w, h);
#endif
	}


	long getTimeStart() {
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - _timeStart;
	}

	void drawUpdateStatus(int timeFromStart = 2000) {
		if (isReloaded() && getTimeStart() < timeFromStart)
		{
#if defined(JUCE_APP_VERSION)
#else
			ofBaseRenderer& renderer = *(window.renderer());
			renderer.pushStyle();
			renderer.pushMatrix();

			float prc = float(getTimeStart() % timeFromStart) / timeFromStart;
			float size = 60;

			renderer.translate(window.getWidth() / 2, window.getHeight() / 2);
			renderer.setColor(0, 0, 0, 255 * (1.0 - prc));
			renderer.drawCircle(0, 0, 0, size / 2);
			renderer.pushMatrix(); 
			renderer.rotateDeg(2 * 360 * prc);
			renderer.translate(-size / 20 / 2, -size / 2);
			renderer.setColor(255, 255, 255, 255 * (1.0 - prc));
			renderer.drawRectangle(0, 0, 0, size / 20, size / 2);
			renderer.popMatrix();

			renderer.popMatrix();
			renderer.popStyle();
#endif
		}
	}

	void drawLog(int maxLines = 40) {
#if defined(JUCE_APP_VERSION)
#else
		ofBaseRenderer& renderer = *(window.renderer());
		renderer.drawString(_log, 20, 20, 0);
#endif
		if (std::count(_log.begin(), _log.end(), '\n') > maxLines) {
			_log = "";
		}
	} 
};


