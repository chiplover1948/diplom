#How to build on Linux systems:
##1. Native library
```
	automake --add-missing
	./configure
	make
```
##2. JavaScript library using Emscripten
```
	automake --add-missing
	emconfigure ./configure --enable-javascript
	emmake make
```
