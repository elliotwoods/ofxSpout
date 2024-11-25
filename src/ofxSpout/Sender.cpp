#include "Sender.h"

#include "Utils.h"

#include "ofFbo.h"

#include <gl/glew.h>
#include "SpoutSender.h"

namespace ofxSpout {
	//----------
	Sender::Sender() {
		this->spoutSender = nullptr;
	}

	//----------
	Sender::~Sender() {
		this->release();
	}

	//----------
	bool Sender::init(std::string channelName, const ofTexture& texture) {
		return this->init(channelName, texture.getWidth(), texture.getHeight(), texture.getTextureData().glInternalFormat);
	}

	//----------
	bool Sender::init(std::string channelName, int initialWidth, int initialHeight, int glFormat) {
		//unitialise any existing sender
		this->release();

		try {
			this->spoutSender = new SpoutSender();

			this->glFormat = glFormat;

			//create the sender, and allow for Spout to change our channel name
			this->spoutSender->SetSenderName(channelName.c_str());
			this->spoutSender->SetSenderFormat(ofxSpout::toDXFormat(glFormat));

			return true;
		}
		catch (const char * e) {
			ofLogError(__FUNCTION__) << e;
			this->release();
			return false;
		}
	}

	//----------
	void Sender::release() {
		if (this->isInitialized()) {
			//free the sender, and reset our local settings
			this->spoutSender->ReleaseSender();
			delete this->spoutSender;
			this->spoutSender = nullptr;
		}
	}

	//----------
	bool Sender::isInitialized() const {
		if (this->spoutSender) {
			return true;
		}
		else {
			return false;
		}
	}
	
	//----------
	bool Sender::send(const ofTexture & texture) {
		if (!this->isInitialized()) {
			ofLogError(__FUNCTION__) << "Sender not initialised";
			return false;
		}

		try {
			this->spoutSender->SendTexture(texture.getTextureData().textureID
				, texture.getTextureData().textureTarget
				, texture.getWidth(), texture.getHeight());
			return true;
		}
		catch (const char * e) {
			ofLogError(__FUNCTION__) << e;
			return false;
		}
	}

	//-----------
	std::string Sender::getChannelName() const {
		return this->channelName;
	}

	//-----------
	bool Sender::setVerticalSync(bool verticalSync) {
		try {
			if (!this->isInitialized()) {
				throw("Not initialized");
			}
			if (!this->spoutSender->SetVerticalSync(verticalSync)) {
				throw("Can't set vertical sync");
			}
			return true;
		}
		catch (const char * e) {
			ofLogError(__FUNCTION__) << e;
			return false;
		}
	}

	//-----------
	bool Sender::getVerticalSync() {
		try {
			if (!this->isInitialized()) {
				throw("Not initialised");
			}
			return this->spoutSender->GetVerticalSync();
		}
		catch (const char * e) {
			ofLogError(__FUNCTION__) << e;
			return false;
		}
	}
};
