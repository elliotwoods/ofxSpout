#pragma once

#include "ofLog.h"
#include "ofTexture.h"
#include <string>
#include "SpoutReceiver.h"

namespace ofxSpout {
	class Receiver {
	public:
		Receiver();
		~Receiver();

		bool init(std::string channelName = "", int textureFormat = GL_RGBA);
		void release();
		bool isInitialized() const;

		bool receive(ofTexture &); // will automatically allocate the texture

		bool selectSenderPanel();

		std::string getChannelName() const;
		float getWidth() const;
		float getHeight() const;

	protected:
		SpoutReceiver * spoutReceiver;

		int textureFormat;
	};
}
