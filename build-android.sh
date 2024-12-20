export  ANDROID_NDK_ROOT=~/Library/Android/sdk/ndk/27.0.12077973
export CARGO_NDK_ANDROID_PLATFORM=21

make android_local
make clean
cargo ndk -t arm64-v8a -t armeabi-v7a build --release
make copy
