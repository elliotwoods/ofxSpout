#pragma once

#include "ofTexture.h"
#include <string>
#include "SpoutSender.h"

namespace ofxSpout {
	class Sender {
	public:
		Sender();
		~Sender();

		bool init(string channelName, int initialWidth = 64, int initialHeight = 64);
		void release();
		bool isInitialized() const;

		bool send(const ofTexture &);

		string getChannelName() const;
		bool setVerticalSync(bool);
		bool getVerticalSync();
	protected:
		string channelName;

		SpoutSender * spoutSender;
		int width, height;
	};
}