#include "Utils.h"
#include "ofGLUtils.h"

namespace ofxSpout {
	//----------
	int toGLFormat(const DXGI_FORMAT & format) {
		switch (format) {
		case DXGI_FORMAT_R32G32B32A32_FLOAT:
			return GL_RGBA32F;
		case DXGI_FORMAT_R16G16B16A16_FLOAT:
			return GL_RGBA16F;
		case DXGI_FORMAT_R16G16B16A16_SNORM: 
			return GL_RGBA16;
		case DXGI_FORMAT_R10G10B10A2_UNORM: 
			return GL_RGB10_A2;
		case DXGI_FORMAT_R8G8B8A8_UNORM: 
			return GL_RGBA8;
		case DXGI_FORMAT_B8G8R8A8_UNORM:
			return GL_BGRA;
		default:
			return 0;
		}
	}
}