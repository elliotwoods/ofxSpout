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
	bool Receiver::init(string channelName) {
		this->release();

		try {
			this->spoutReceiver = new SpoutReceiver();
			//name provided, so let's use it
			char mutableName[256];
			strcpy_s(mutableName, channelName.size() + 1, channelName.c_str());
			unsigned int mutableWidth, mutableHeight;
			if (!this->spoutReceiver->CreateReceiver(mutableName, mutableWidth, mutableHeight, channelName.empty())) {
				throw("Can't create receiver");
			}
			this->channelName = string(mutableName);
			this->width = mutableWidth;
			this->height = mutableHeight;
			return true;
		}
		catch (const char * e) {
			ofLogError("ofxSpout::Sender::init") << "Channel : " << channelName << " : " << e;
			this->release();
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
			if (texture.getWidth() != this->width || texture.getHeight() != this->height) {
				int format = texture.isAllocated() ? texture.getTextureData().glInternalFormat : this->defaultFormat;
				texture.allocate(width, height, format);
			}

			//pull data into the texture (keep any existing fbo attachments)
			GLint drawFboId = 0;
			glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &drawFboId);
			if (!this->spoutReceiver->ReceiveTexture(mutableName, mutableWidth, mutableHeight, texture.getTextureData().textureID, texture.getTextureData().textureTarget, false, drawFboId)) {
				throw("Can't receive texture");
			}

			//update our local settings incase anything changed
			this->channelName = mutableName;
			this->width = mutableWidth;
			this->height = mutableHeight;

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
			this->spoutReceiver->SelectSenderPanel("Select sender");
			return true;
		}
		catch (const char * e) {
			ofLogError("ofxSpout::Receiver::selectSenderPanel") << e;
			return false;
		}
	}
	
	//-----------
	string Receiver::getChannelName() const {
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