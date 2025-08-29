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

# Installation
There are premade building scripts in the scripts folder for the following system.

## Windows
1. Install [rustc]("https://www.rust-lang.org/")
2. Install [Microsoft C++ Build Tools]("https://visualstudio.microsoft.com/visual-cpp-build-tools/")
3. Open cmd and install WSL
```
wsl --install
```
4. Open WSL and install Make
```
sudo apt install make
```

### Windows
1. Add build targets in cmd with
```
rustup target add x86_64-pc-windows-msvc
rustup target add i686-pc-windows-msvc
```

### Android
1. Install [Android Studio]("https://developer.android.com/studio") and download the latest NDK.
2. Install [cargo-ndk]("https://github.com/bbqsrc/cargo-ndk") with cargo in cmd
```
cargo install cargo-ndk
```
3. Install build targets
```
rustup target add aarch64-linux-android
rustup target add armv7-linux-androideabi
```

## Mac
1. Install [rustc]("https://www.rust-lang.org/")
2. Install [Xcode & Development tools]("https://apps.apple.com/us/app/xcode/id497799835")

### iOS, iPad, Mac Native & Catalyst
1. Install build targets
```
rustup target add aarch64-apple-ios
rustup target add x86_64-apple-ios-macabi
```

### Android
1. Install [Android Studio]("https://developer.android.com/studio") and download the latest NDK.
2. Install [cargo-ndk]("https://github.com/bbqsrc/cargo-ndk") with cargo in cmd
```
cargo install cargo-ndk
```
3. Install build targets
```
rustup target add aarch64-linux-android
rustup target add armv7-linux-androideabi
```

# Building
Inside the scripts folder

## Windows
### Windows
1. Use the build_win.cmd or build_win_local.cmd
### Android
1. Use the build_android.cmd or build_android_local.cmd
## Mac
### iOS, iPad, Mac Native & Catalyst
1. Use the build_ios.cmd or build_ios_local.cmd
### Android
1. Use the build_android.sh or build_android_local.sh

# Usage

### iOS, iPad, Mac Native & Catalyst
Catalyst is broken, due to std path issues needs internal rules folder
1. Bundle the MathCat library into your c++ project
2. Add the Rules folder unziped into the supporting files.
3. Change build rules for Rules folder to "Apply Once to Folder"
4. Provide MathCat with the [[NSBundle mainBundle] resourcePath] to the Rules folder

### Android
1. Bundle the MathCat library into your c++ project
2. Add the Rules folder zipped into the assets folder.
3. Add code for unzipping the Rules.zip to a accessible folder (ie *appspace*/Rules)
4. Provide MathCat with the path to the Rules folder

### Windows
1. Bundle the MathCat library into your c++ project
2. Add Rules unzipped to an accessible folder space (ie deployment_folder)
4. Provide MathCat with the path to the Rules folder