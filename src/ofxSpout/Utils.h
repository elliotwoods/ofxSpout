#pragma once
#include <string>
#include "dxgiformat.h"

namespace ofxSpout {
	int toGLFormat(const DXGI_FORMAT &);
	DXGI_FORMAT toDXFormat(const int &);

	std::string toDXString(const DXGI_FORMAT &);
	std::string toGLString(const int &);
}