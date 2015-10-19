#include "Sender.h"

#include "SpoutDLL.h"

namespace ofxSpout {
	//----------
	Sender::Sender() {
		this->initialized = false;
	}

	//----------
	Sender::~Sender() {
		this->release();
	}

	//----------
	void Sender::init(string channelName) {
		this->init(channelName, 512, 512);
	}

	//----------
	void Sender::init(string channelName, int width, int height) {
		this->release();

		char * mutableName = new char[channelName.size() + 1];
		strcpy_s(mutableName, channelName.size() + 1, channelName.c_str());
		auto success = Spout2::CreateSender(mutableName, width, height);

		if (success) {
			this->initialized = true;
			this->channelName = string(mutableName);
		}
		else {
			this->initialized = false;
		}

		delete[] mutableName;
	}

	//----------
	void Sender::release() {
		if (this->initialized) {
			Spout2::ReleaseSender();
			this->initialized = false;
		}
	}

	//----------
	void Sender::send(const ofTexture & texture) {
		Spout2::SendTexture(texture.getTextureData().textureID, GL_TEXTURE_2D, texture.getWidth(), texture.getHeight());
		Spout2::UpdateSender(this->channelName)
	}
};