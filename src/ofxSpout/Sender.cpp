#include "Sender.h"

#include "Utils.h"

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
	bool Sender::init(std::string channelName, const ofTexture& texture) {
		return this->init(channelName, texture.getWidth(), texture.getHeight(), texture.getTextureData().glInternalFormat);
	}

	//----------
	bool Sender::init(std::string channelName, int initialWidth, int initialHeight, int glFormat) {
		//unitialise any existing sender
		this->release();

		try {
			this->spoutSender = new SpoutSender();

			this->width = initialWidth;
			this->height = initialHeight;
			this->glFormat = glFormat;

			//create the sender, and allow for Spout to change our channel name
			if (!this->spoutSender->SetupSender(channelName.c_str(), this->width, this->height, toDXFormat(this->glFormat))) {
				throw("Can't create sender");
			}
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
	bool Sender::send(const ofTexture & texture) {
		if (!this->isInitialized()) {
			ofLogError("ofxSpout::Sender::send") << "Sender not initialised";
			return false;
		}

		try {
			//check if the sender matches the settings of the texture
			if (this->spoutSender->GetWidth() != texture.getWidth() || this->spoutSender->GetHeight() != texture.getHeight()) {
				//update the sender to match local settings
				this->spoutSender->Update(this->width, this->height);
				this->width = texture.getWidth();
				this->height = texture.getHeight();
			}

			//send texture and retain any fbo bound for drawing
			GLint drawFboId = 0;
			glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &drawFboId);
			this->spoutSender->SendTextureData(texture.getTextureData().textureID, texture.getTextureData().textureTarget, drawFboId);
			return true;
		}
		catch (const char * e) {
			ofLogError("ofxSpout::Sender::send") << e;
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
