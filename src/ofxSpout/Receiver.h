#pragma once

#include "ofLog.h"
#include "ofTexture.h"
#include <string>
#include <vector>
#include "SpoutReceiver.h"

namespace ofxSpout {
	class Receiver {
	public:
		Receiver();
		~Receiver();

		bool init(std::string channelName = "");
		void release();
		bool isInitialized() const;

		bool receive(ofTexture&); // will automatically allocate the texture

		bool selectSenderPanel();

		std::string getChannelName() const;
		float getWidth() const;
		float getHeight() const;

		std::vector<std::string> getAvailableSenders;


	protected:
		SpoutReceiver* spoutReceiver;

		const int defaultFormat; // = GL_RGBA. If you're using anything else then make sure to allocate your texture manually
	};
}
