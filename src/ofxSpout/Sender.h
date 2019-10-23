#pragma once

#include "ofLog.h"
#include "ofTexture.h"
#include <string>
#include "SpoutSender.h"

namespace ofxSpout {
	class Sender {
	public:
		Sender();
		~Sender();

		bool init(std::string channelName, const ofTexture& texture);
		bool init(std::string channelName, int initialWidth = 64, int initialHeight = 64, int glFormat = GL_RGBA);
		void release();
		bool isInitialized() const;

		bool send(const ofTexture &);

		std::string getChannelName() const;
		bool setVerticalSync(bool);
		bool getVerticalSync();
	protected:
		std::string channelName;

		SpoutSender * spoutSender;
		int width, height;
		int glFormat;
	};
}
