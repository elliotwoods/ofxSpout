#include "Receiver.h"
#include "ofGraphics.h"
#include "Utils.h"

namespace ofxSpout {
	//----------
	Receiver::Receiver() :
		defaultFormat(GL_RGBA) {
		this->spoutReceiver = nullptr;
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
				this->spoutReceiver->SetReceiverName(channelName.c_str());
			}

			return true;
		}
		catch (const char * e) {
			ofLogError(__FUNCTION__) << "Channel : " << channelName << " : " << e;
			return false;
		}
	}

	//----------
	void Receiver::release() {
		if (this->isInitialized()) {
			this->spoutReceiver->ReleaseReceiver();
			delete this->spoutReceiver;
			this->spoutReceiver = nullptr;
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

			//check if the texture is allocated correctly, if not, allocate it
			if (this->spoutReceiver->IsUpdated()) {
				texture.allocate(this->spoutReceiver->GetSenderWidth(), this->spoutReceiver->GetSenderHeight(), GL_RGBA);
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
			ofLogError(__FUNCTION__) << e;
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
			ofLogError(__FUNCTION__) << e;
			return false;
		}
	}
	
	//-----------
	std::string Receiver::getChannelName() const {
		if (this->isInitialized()) {
			return this->spoutReceiver->GetSenderName();
		}
		return "";
	}

	//----------
	float Receiver::getWidth() const {
		if (this->isInitialized()) {
			return this->spoutReceiver->GetSenderWidth();
		}
		return 0;
	}

	//----------
	float Receiver::getHeight() const {
		if (this->isInitialized()) {
			return this->spoutReceiver->GetSenderHeight();
		}
		return 0;
	}
	vector<std::string> Receiver::getAvailableSenders() {
		if (this.isInitialized()) {
			int n_senders {this->spoutReceiver->GetSenderCount()};
			vector<std::string> senders {vector<std::string>()};
			for (int i = 0; i < n_senders; i++) {
				char name[100]{0};
				this->spoutReceiver->GetSender(i, &name);
				senders.push_back((std::string)senders);
			}
			return senders;
		}
		return vector<std::string>();
	}
}
