# ofxSpout
Implementation of Spout v2 for sharing textures between applications (including DX and GL)

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