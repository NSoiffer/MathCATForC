# C/C++ Interface for MathCAT

<img src="logo.png" style="position: relative; top: 16px; z-index: -1;"> 
<!-- vertical alignment does seems to get pushed into the github front page view, so trying a separate line -->
<div style=position: relative; top: -1rem >is a library that supports conversion of MathML to speech and braille among other things.</div>

This project adds a C/C++ interface to MathCAT.
Visit [the MathCAT project page](https://nsoiffer.github.io/MathCAT/) for more info or if you want to play around with MathCAT's capabilities, [try out the demo](https://nsoiffer.github.io/MathCATDemo/).

Warning: the interface is subject to change. This is my first attempt to add a C interface to Rust code. There are two issues I'm not happy with the current interface:
* Because strings are passed back and forth, C code needs to free the Rust strings returned. It's a bit ugly, but that's C's memory management...
* Errors can happen and I didn't see a good way to handle this. The current method is to return an empty string to indicate an error. This means the calling code should check this condition after each condition. Call GetError() to find out what the error is.

To use this project:
1. build the project (`cargo build --release`)
2. this will create a release build in target/release and also create a directory `c-example` with `mathcat.h` and the MathCAT `Rules` dir in it.
3. try compiling the the sample program `test.cpp` in that directory or create your own using that as an example. See instructions for compiling at the start of that file. 

Alternatively, to avoid having to have a Rust build environment:
download the a prebuilt dll from [releases](https://github.com/NSoiffer/MathCATForC/tags). The Release-_version_-_target_.zip file includes the dll, c-header file, a test/example program, and the Rules directory needed by MathCAT. It is probably best to use the latest release at that link. If you need a target/dll not listed there, file an issue.

