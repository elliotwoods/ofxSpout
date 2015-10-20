# ofxSpout
![Thumbnail](https://raw.githubusercontent.com/elliotwoods/ofxSpout/master/ofxaddonx_thumbnail.png)

Implementation of Spout v2 for sharing textures between applications (including DX and GL)

Tested with:
* openFrameworks 0.9.0 at 3d2c9f2d25c4401274b70e5f68251fd459d8a894
* Visual Studio 2015
* 32bit and 64bit
* Windows 8.1

# Usage

### Sending

```c++
class ofApp {
	ofxSpout::Sender sender;
}
```

```c++
void ofApp::setup() {
	sender.init("My Sender");
	//...
}

void ofApp::update() {
	sender.send(myTexture);
}
```

### Receiving

```c++
class ofApp {
	ofxSpout::Receiver sender;
}
```


```c++
void ofApp::setup() {
	receiver.init();
	//...
}

void ofApp::update() {
	receiver.receive(myTexture);
}
```

# Notes

## Windows Vista

Due to the deprecated `GetVersion` function, I've hardcoded a `true` at https://github.com/elliotwoods/ofxSpout/blob/master/libs/SpoutSDK/src/SpoutDirectX.cpp#L540. If you are running on a version of windows which doesn't have DX11 (i.e. Vista or earlier), then you can either uncomment the function or change the literal to be `false`.
