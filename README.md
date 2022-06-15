# C/C++ Interface for MathCAT

<img src="logo.png" style="position: relative; top: -6px; z-index: -1;vertical-align:middle"> is a library that supports conversion of MathML to speech and braille among other things.

This project adds a C/C++ interface to MathCAT.
Visit [the MathCAT project page](https://nsoiffer.github.io/MathCAT/) for more info or if you want to play around with MathCAT's capabilities, [try out the demo](https://nsoiffer.github.io/MathCATDemo/).

Warning: the interface is subject to change. This is my first attempt to add a C interface to Rust code. There are two issues I'm not happy with the current interface:
* Because strings are passed back and forth, C code needs to free the Rust strings returned. It's a bit ugly, but that's C's memory management...
* Errors can happen and I didn't see a good way to handle this. The current method is to return an empty string to indicate an error. This means the calling code should check this condition after each condition. Call GetError() to find out what the error is.

To use this project:
1. build the project (`cargo build --release`)
2. this will create a release build in target/release and also create a directory `c-example` with `mathcat.h` and the MathCAT `Rules` dir in it.
3. try compiling the the sample program `test.c` in that directory or create your own using that as an example. See some instructions for compiling in that file. 

Alternatively, to avoid having to have a Rust build environment:
1. download the MathCAT for C/C++ crate from https://crates.io/crates/mathcat_c
2. download mathcat.h and the Rules from [here](https://github.com/NSoiffer/MathCATForC/tree/main/c-example)
