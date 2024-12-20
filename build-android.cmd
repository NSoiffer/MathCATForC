@echo off
wsl make android_local

cargo clean
cargo ndk -t arm64-v8a -t armeabi-v7a build --release

wsl make copy
