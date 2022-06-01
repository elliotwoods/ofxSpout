#include "Utils.h"
#include "ofGLUtils.h"

namespace ofxSpout {
	//----------
	int toGLFormat(const DXGI_FORMAT & format) 
	{
		switch (format) 
		{
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

	DXGI_FORMAT toDXFormat(const int & format)
	{
		switch (format)
		{
		case GL_R32F:
		case GL_RG32F:
		case GL_RGB32F:
		case GL_RGBA32F:
			return DXGI_FORMAT_R32G32B32A32_FLOAT;
		case GL_RGBA16F:
			return DXGI_FORMAT_R16G16B16A16_FLOAT;
		case GL_R16:
		case GL_RG16:
		case GL_RGB16:
		case GL_RGBA16:
			return DXGI_FORMAT_R16G16B16A16_SNORM;
		case GL_RGB10_A2:
			return DXGI_FORMAT_R10G10B10A2_UNORM;
		case GL_RGBA8:
			return DXGI_FORMAT_R8G8B8A8_UNORM;
		case GL_BGRA:
			return DXGI_FORMAT_B8G8R8A8_UNORM;
		default:
			return DXGI_FORMAT_UNKNOWN;
		}
	}

	std::string toDXString(const DXGI_FORMAT & format)
	{
		switch (format)
		{
		case DXGI_FORMAT_R32G32B32A32_FLOAT:
			return "R32G32B32A32_FLOAT";
		case DXGI_FORMAT_R16G16B16A16_FLOAT:
			return "R16G16B16A16_FLOAT";
		case DXGI_FORMAT_R16G16B16A16_UNORM:
			return "R16G16B16A16_UNORM";
		case DXGI_FORMAT_R10G10B10A2_UNORM:
			return "R10G10B10A2_UNORM";
		case DXGI_FORMAT_R8G8B8A8_UNORM:
			return "R8G8B8A8_UNORM";
		case DXGI_FORMAT_B8G8R8A8_UNORM:
			return "B8G8R8A8_UNORM";
		default:
			return "UNKNOWN";
		}
	}

	std::string toGLString(const int & format)
	{
		switch (format)
		{
		case GL_R32F:
			return "GL_R32F";
		case GL_RG32F:
			return "GL_RG32F";
		case GL_RGB32F:
			return "GL_RGB32F";
		case GL_RGBA32F:
			return "GL_RGBA32F";
		case GL_RGBA16F:
			return "GL_RGBA16F";
		case GL_R16:
			return "GL_R16";
		case GL_RG16:
			return "GL_RG16";
		case GL_RGB16:
			return "GL_RGB16";
		case GL_RGBA16:
			return "GL_RGBA16";
		case GL_RGB10_A2:
			return "GL_RGB10_A2";
		case GL_RGBA8:
			return "GL_RGBA8";
		case GL_BGRA:
			return "GL_BGRA";
		default:
			return "UNKNOWN";
		}
	}
}