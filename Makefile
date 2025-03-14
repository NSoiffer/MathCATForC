mathcatVersion="0.6.6"

define package
[package]
name = "mathcat_c"
version = $(mathcatVersion)
authors = ["Neil Soiffer <soiffer@alum.mit.edu>"]
edition = "2021"
license = "MIT"
description = "C/C++ interface for MathCAT (for MathCAT info, see crates.io or nsoiffer.github.io/MathCAT)"
resolver = "2"    # allows different build dependency features

endef
export package


define dependencies
[dependencies.mathcat]
version = $(mathcatVersion)
# for testing MathCAT without having to publish a new version (change here and in build-dependencies)
path = "../MathCAT/"
features = ["include-zip"]   # for building, we want the zip files so we can include them separately

endef
export dependencies
define dependencies_local
[dependencies.mathcat]
version = $(mathcatVersion)
# for testing MathCAT without having to publish a new version (change here and in build-dependencies)
path = "../MathCAT/"
features = ["include-zip"]   # for building, we want the zip files so we can include them separately

endef
export dependencies_local

define builder
[build-dependencies]
zip = { version = "2.1", default-features = false, features = ["bzip2"] }
cbindgen = "0.24"
# for testing MathCAT without having to publish a new version (change two occurences)
# mathcat = {path = "../MathCAT/", version = $(mathcatVersion), features = ["include-zip"]}
#mathcat = { version = $(mathcatVersion), features = ["include-zip"]} # for building, we want the zip files so we can include them separately

[target.'cfg(target_os = "windows")'.build-dependencies]
embed-resource = "2.5"

endef
export builder

define builder_local
[build-dependencies]
zip = { version = "2.1", default-features = false, features = ["bzip2"] }
cbindgen = "0.24"
# for testing MathCAT without having to publish a new version (change two occurences)
mathcat = {path = "../MathCAT/", version = $(mathcatVersion), features = ["include-zip"]}
# mathcat = { version = $(mathcatVersion), features = ["include-zip"]} # for building, we want the zip files so we can include them separately

[target.'cfg(target_os = "windows")'.build-dependencies]
embed-resource = "2.5"

endef
export builder_local



define profile
[profile.release]
debug = false
codegen-units = 1
lto = true
opt-level = "z"  # Optimize for size.

endef
export profile


define lib_static
[lib]
name = "mathcat_c"
crate-type = ["staticlib"]      # Creates static lib

endef
export lib_static


define lib_dynamic
[lib]
name = "libmathcat_c"
crate-type = ["cdylib"]      # Creates dynamic lib

endef
export lib_dynamic

define start
	@echo "$$package" > ${cargofile}
	@echo "$$dependencies" >> ${cargofile}
endef

define last
	@echo "$$builder" >> ${cargofile}
	@echo "$$profile" >> ${cargofile}
endef


define start_local
	@echo "$$package" > ${cargofile}
	@echo "$$dependencies_local" >> ${cargofile}
endef

define last_local
	@echo "$$builder_local" >> ${cargofile}
	@echo "$$profile" >> ${cargofile}
endef


cargofile="Cargo.toml"


clean:
	cargo clean

delete:
	rm -rf ./MathCat
	
.PHONY: all

copy:
	./copy.sh "v${mathcatVersion}"

ios:
	$(call start);
	@echo "$${lib_static}" >>  ${cargofile}
	$(call last);
	rustc --target aarch64-apple-ios --print deployment-target
	cargo build --release --lib --target aarch64-apple-ios  --target-dir "./target/mathcat-ios-arm64"

ios_local:
	$(call start_local);
	@echo "$${lib_static}" >>  ${cargofile}
	$(call last_local);
	rustc --target aarch64-apple-ios --print deployment-target
	cargo build --release --lib --target aarch64-apple-ios  --target-dir "./target/mathcat-ios-arm64"


# Catalyst version still not working sadly, it gets stuck at is_dir when setting the Rules dir.
catalyst:
	$(call start);
	@echo "$${lib_static}" >>  ${cargofile}
	$(call last);
	rustc --target x86_64-apple-ios-macabi --print deployment-target
	cargo +nightly build -Z build-std --release --lib --target x86_64-apple-ios-macabi --target-dir "./target/mathcat-catalyst-x64"
	
catalyst_local:
	$(call start_local);
	@echo "$${lib_static}" >>  ${cargofile}
	$(call last_local);
	rustc --target x86_64-apple-ios-macabi --print deployment-target
	cargo +nightly build -Z build-std --release --lib --target x86_64-apple-ios-macabi --target-dir "./target/mathcat-catalyst-x64"
	

android:
	$(call start);
	@echo "$${lib_static}" >>  ${cargofile}
	$(call last);
	
android_local:
	$(call start_local);
	@echo "$${lib_static}" >>  ${cargofile}
	$(call last_local);
	
# Windows for windows
windows:
	$(call start);
	@echo "$${lib_dynamic}" >>  ${cargofile}
	$(call last);
	
windows_local:
	$(call start_local);
	@echo "$${lib_dynamic}" >>  ${cargofile}
	$(call last_local);
	
# Build from mac to windows
windows-unix:
	$(call start);
	@echo "$${lib_dynamic}" >>  ${cargofile}
	$(call last);
	cargo build --lib --release --target x86_64-pc-windows-gnu --target-dir "./target/mathcat-windows-x64"

		
