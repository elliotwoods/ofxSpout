#pragma once

#include "ofTexture.h"
#include <string>
#include "SpoutReceiver.h"

namespace ofxSpout {
	class Receiver {
	public:
		Receiver();
		~Receiver();

		bool init(string channelName = "");
		void release();
		bool isInitialized() const;

		bool receive(ofTexture &); // will automatically allocate the texture

		bool selectSenderPanel();

		string getChannelName() const;
		float getWidth() const;
		float getHeight() const;
	protected:
		string channelName;

		SpoutReceiver * spoutReceiver;
		int width;
		int height;
		const int defaultFormat; // = GL_RGBA. If you're using anything else then make sure to allocate your texture manually
	};
}