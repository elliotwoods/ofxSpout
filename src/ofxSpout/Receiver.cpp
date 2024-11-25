#include "Receiver.h"
#include "ofGraphics.h"
#include "Utils.h"
#include "ofMain.h"

#include "gl/glew.h"
#include "SpoutReceiver.h"

namespace ofxSpout {
	//----------
	Receiver::Receiver() :
		textureFormat(GL_RGBA) {
		this->spoutReceiver = nullptr;
	}

	//----------
	Receiver::~Receiver() {
		this->release();
	}

	//----------
	bool Receiver::init(std::string channelName, int textureFormat) {
		this->release();

		this->textureFormat = textureFormat;

		try {
			this->spoutReceiver = new SpoutReceiver();
			//name provided, so let's use it
			if (!channelName.empty()) {
				this->spoutReceiver->SetReceiverName(channelName.c_str());
			}

			return true;
		}
		catch (const char* e) {
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
	bool Receiver::receive(ofTexture& texture) {
		try {
			//check if we're initialised
			if (!this->isInitialized()) {
				throw("Not initialized");
			}

<<<<<<< HEAD
			if (this->spoutReceiver->ReceiveTexture()) {
				// Update the receiving texture if the received size has changed
				if (this->spoutReceiver->IsUpdated()) {
					texture.allocate(this->spoutReceiver->GetSenderWidth(), this->spoutReceiver->GetSenderHeight(), this->spoutReceiver->GLDXformat());
					return false; // Return now because the texture will empty
				}
				// Bind to get access to the shared texture
				if (this->spoutReceiver->BindSharedTexture()) {
					// Copy from the shared texture 
					this->spoutReceiver->CopyTexture(this->spoutReceiver->GetSharedTextureID(), GL_TEXTURE_2D,
						texture.getTextureData().textureID,
						texture.getTextureData().textureTarget,
						this->spoutReceiver->GetSenderWidth(), this->spoutReceiver->GetSenderHeight());
					// Un-bind to release access to the shared texture
					this->spoutReceiver->UnBindSharedTexture();
				}
				else {
					return false;
				}
=======
			//check if the texture is allocated correctly, if not, allocate it
			if (this->spoutReceiver->IsUpdated()) {
				texture.allocate(this->spoutReceiver->GetSenderWidth(), this->spoutReceiver->GetSenderHeight(), this->textureFormat);
>>>>>>> 7e6620087f17bc72b96ba5da2c2c2286ed5ecc12
			}
			else {
				return false;
			}

			return true;
		}
		catch (const char* e) {
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
		catch (const char* e) {
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

	std::vector<std::string> Receiver::getAvailableSenders() {
		if (this->isInitialized()) {
			int n_senders{ this->spoutReceiver->GetSenderCount() };
			std::vector<std::string> senders;
			for (int i = 0; i < n_senders; i++) {
				char name[100]{'\0'};
				this->spoutReceiver->GetSender(i, name, 100); 
				std::string name_s{ name };
				senders.push_back(name_s);
			}
			return senders;
		}
		return std::vector<std::string>();
	}
}
