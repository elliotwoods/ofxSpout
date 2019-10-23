# ofxSpout
![Thumbnail](https://raw.githubusercontent.com/elliotwoods/ofxSpout/master/ofxaddonx_thumbnail.png)

Implementation of [Spout v2](http://spout.zeal.co/) for sharing textures between applications (including DX and GL).

Tested with:
* openFrameworks 0.10.1 at a1deaf147bf0f9bbd2282db880afd23fd4cb7a7f
* Visual Studio 2017
* 32bit and 64bit
* Windows 10
* Spout v2.007

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
