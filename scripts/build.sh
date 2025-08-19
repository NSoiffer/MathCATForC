#!/bin/bash

cd "$(dirname "$0")/.."
if uname -r | tr '[:upper:]' '[:lower:]' | grep -q "microsoft"; then
	cmd.exe /c cargo clean
else
    cargo clean
fi
cd -

if [ "$1" = "ios" ]; then

	if [ "$2" = "local" ]; then
		make ios_local
	else
		make ios
	fi
          
    cd "$(dirname "$0")/.."
    rustc --target aarch64-apple-ios --print deployment-target
    cargo build --release --lib --target aarch64-apple-ios  --target-dir "./target/mathcat-ios-arm64"
    cd -
    
    if [ "$2" = "local" ]; then
        make catalyst_local
    else
        make catalyst
    fi
     
    cd "$(dirname "$0")/.."
    rustc --target x86_64-apple-ios-macabi --print deployment-target
    cargo +nightly build -Z build-std --release --lib --target x86_64-apple-ios-macabi --target-dir "./target/mathcat-catalyst-x64"
    cd -
	

elif [ "$1" = "windows" ]; then

	if [ "$2" = "local" ]; then
		make windows_local
	else
		make windows
	fi	
	
	cd "$(dirname "$0")/.."
	if uname -r | tr '[:upper:]' '[:lower:]' | grep -q "microsoft"; then
		cmd.exe /c cargo build --lib --release --target x86_64-pc-windows-msvc --target-dir "./target/mathcat-windows-x64"
		cmd.exe /c cargo build --lib --release --target i686-pc-windows-msvc --target-dir "./target/mathcat-windows-x86"
	else
		cargo build --lib --release --target x86_64-pc-windows-msvc --target-dir "./target/mathcat-windows-x64"
		cargo build --lib --release --target i686-pc-windows-msvc --target-dir "./target/mathcat-windows-x86"
	fi
	cd -
	

elif [ "$1" = "android" ]; then
	export ANDROID_NDK_ROOT=~/Library/Android/sdk/ndk #Use the default android studio path
	export CARGO_NDK_ANDROID_PLATFORM=21

	if [ "$2" = "local" ]; then
		make android_local
	else
		make android
	fi
	
	cd "$(dirname "$0")/.."
	if uname -r | tr '[:upper:]' '[:lower:]' | grep -q "microsoft"; then
		cmd.exe /c cargo ndk -t arm64-v8a -t armeabi-v7a build --release
	else
		cargo ndk -t arm64-v8a -t armeabi-v7a build --release
	fi
	cd -
	
else
	echo "Please add ios, android or windows"
fi


make copy

