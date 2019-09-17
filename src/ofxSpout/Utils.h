#pragma once
#include "dxgiformat.h"

namespace ofxSpout {
	int toGLFormat(const DXGI_FORMAT &);
	DXGI_FORMAT toDXFormat(const int &);
}