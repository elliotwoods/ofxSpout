#include "Receiver.h"
#include "ofGraphics.h"
#include "Utils.h"

namespace ofxSpout {
	//----------
	Receiver::Receiver() :
		defaultFormat(GL_RGBA) {
		this->spoutReceiver = nullptr;
		this->width = 0;
		this->height = 0;
	}

	//----------
	Receiver::~Receiver() {
		this->release();
	}

	//----------
	bool Receiver::init(std::string channelName) {
		this->release();

		try {
			this->spoutReceiver = new SpoutReceiver();
			//name provided, so let's use it
			if (!channelName.empty()) {
				this->channelName = channelName;
				this->spoutReceiver->SetReceiverName(channelName.c_str());
			}
			return true;
		}
		catch (const char * e) {
			ofLogError("ofxSpout::Sender::init") << "Channel : " << channelName << " : " << e;
			return false;
		}
	}

	//----------
	void Receiver::release() {
		if (this->isInitialized()) {
			this->spoutReceiver->ReleaseReceiver();
			delete this->spoutReceiver;
			this->spoutReceiver = nullptr;
			this->width = 0;
			this->height = 0;
		}
	}

	//----------
	bool Receiver::isInitialized() const {
		if (this->spoutReceiver) {
			return true;
		}
		else {
			return false;
		}
	}

	//----------
	bool Receiver::receive(ofTexture & texture) {
		try {
			//check if we're initialised
			if (!this->isInitialized()) {
				throw("Not initialized");
			}

			//prepare the channel name, allow it to be changed if different channels are available
			char mutableName[256];
			unsigned int mutableWidth, mutableHeight;
			strcpy_s(mutableName, this->channelName.size() + 1, this->channelName.c_str());

			//check if the texture is allocated correctly, if not, allocate it
			if (this->spoutReceiver->IsUpdated()) {
				this->channelName = this->spoutReceiver->GetSenderName();
				this->width = this->spoutReceiver->GetSenderWidth();
				this->height = this->spoutReceiver->GetSenderHeight();
				texture.allocate(this->width, this->height, GL_RGBA);
			}

			//pull data into the texture (keep any existing fbo attachments)
			GLint drawFboId = 0;
			glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &drawFboId);
			if (!this->spoutReceiver->ReceiveTextureData(texture.getTextureData().textureID, texture.getTextureData().textureTarget, drawFboId)) {
				throw("Can't receive texture");
			}

			return true;
		}
		catch (const char * e) {
			ofLogError("ofxSpout::Receiver::receive") << e;
			return false;
		}
	}

	//----------
	bool Receiver::selectSenderPanel() {
		try {
			if (!this->isInitialized()) {
				throw("Not initialized");
			}
			this->spoutReceiver->SelectSender();
			return true;
		}
		catch (const char * e) {
			ofLogError("ofxSpout::Receiver::selectSenderPanel") << e;
			return false;
		}
	}
	
	//-----------
	std::string Receiver::getChannelName() const {
		return this->channelName;
	}

	//----------
	float Receiver::getWidth() const {
		return this->width;
	}

	//----------
	float Receiver::getHeight() const {
		return this->height;
	}
}