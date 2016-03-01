#include "Sender.h"

#include "ofFbo.h"

namespace ofxSpout {
	//----------
	Sender::Sender() {
		this->spoutSender = nullptr;
		this->width = 0;
		this->height = 0;
	}

	//----------
	Sender::~Sender() {
		this->release();
	}

	//----------
	bool Sender::init(string channelName, int initialWidth, int initialHeight) {
		//unitialise any existing sender
		this->release();

		try {
			this->spoutSender = new SpoutSender();

			this->width = initialWidth;
			this->height = initialHeight;

			//create the sender, and allow for Spout to change our channel name
			char mutableName[256];
			strcpy_s(mutableName, channelName.size() + 1, channelName.c_str());
			if (!this->spoutSender->CreateSender(mutableName, this->width, this->height)) {
				throw("Can't create sender");
			}
			this->channelName = string(mutableName);
			return true;
		}
		catch (const char * e) {
			ofLogError("ofxSpout::Sender::init") << e;
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
			this->width = 0;
			this->height = 0;
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
	bool Sender::send(const ofTexture & texture, bool invert) {
		try {
			if (!this->isInitialized()) {
				throw("Not initialised");
			}

			//check if the sender matches the settings of the texture
			if (this->width != texture.getWidth() || this->height != texture.getHeight()) {
				this->width = texture.getWidth();
				this->height = texture.getHeight();

				//update the sender to match local settings
				char mutableName[256];
				strcpy_s(mutableName, channelName.size() + 1, channelName.c_str());
				if (!this->spoutSender->UpdateSender(mutableName, this->width, this->height)) {
					throw("Can't update sender");
				}
				this->channelName = string(mutableName);
			}

			//send texture and retain any fbo bound for drawing
			GLint drawFboId = 0;
			glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &drawFboId);
			this->spoutSender->SendTexture(texture.getTextureData().textureID, texture.getTextureData().textureTarget, this->width, this->height, invert, drawFboId);
			return true;
		}
		catch (const char * e) {
			ofLogError("ofxSpout::Sender::send") << e;
			return false;
		}
	}

	//-----------
	string Sender::getChannelName() const {
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
			ofLogError("ofxSpout::Sender::setVerticalSync") << e;
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
			ofLogError("ofxSpout::Sender::getVerticalSync") << e;
			return false;
		}
	}
};