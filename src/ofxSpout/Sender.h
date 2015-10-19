#pragma once

#include "ofTexture.h"
#include <string>

namespace ofxSpout {
	class Sender {
	public:
		Sender();
		~Sender();
		void init(string channelName);
		void init(string channelName, int width, int height);
		void release();

		void send(const ofTexture &);
	protected:
		bool initialized;
		string channelName;
	};
}