@echo off
wsl make windows_local

cargo clean
cargo build --lib --release --target x86_64-pc-windows-msvc --target-dir "./target/mathcat-windows-x64"
cargo build --lib --release --target i686-pc-windows-msvc --target-dir "./target/mathcat-windows-x86"

wsl make copy