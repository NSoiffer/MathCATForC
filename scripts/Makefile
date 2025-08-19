mathcatVersion="0.7.0"

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
[features]
"include-zip" = ["mathcat/include-zip"]  # include the zip files in the build

[dependencies]
mathcat = {version = $(mathcatVersion)}
# mathcat = {path= "../MathCAT/"}  # for building, we want the zip files so we can include them separately

endef
export dependencies
define dependencies_local
[features]
"include-zip" = ["mathcat/include-zip"]  # include the zip files in the build

[dependencies]
# mathcat = {version = $(mathcatVersion)}
mathcat = {path= "../MathCAT/"}  # for building, we want the zip files so we can include them separately

endef
export dependencies_local

define builder
[build-dependencies]
cfg-if = "1.0.1"
zip = { version = "4.3", default-features = false, features = ["bzip2"] }
cbindgen = "0.29"
# for testing MathCAT without having to publish a new version (change two occurrences)
mathcat = {version = $(mathcatVersion), features = ["include-zip"]}     # for building, we want the zip files so we can include them separately
# mathcat = {path = "../MathCAT/", features = ["include-zip"]}     # for building, we want the zip files so we can include them separately

[target.'cfg(target_os = "windows")'.build-dependencies]
embed-resource = "2.5"

endef
export builder

define builder_local
[build-dependencies]
cfg-if = "1.0.1"
zip = { version = "4.3", default-features = false, features = ["bzip2"] }
cbindgen = "0.29"
# for testing MathCAT without having to publish a new version (change two occurrences)
# mathcat = {version = $(mathcatVersion), features = ["include-zip"]}     # for building, we want the zip files so we can include them separately
mathcat = {path = "../MathCAT/", features = ["include-zip"]}     # for building, we want the zip files so we can include them separately

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


cargofile="../Cargo.toml"


clean:
	cd "$(dirname "$0")/.."
	cargo clean
	cd -

delete:
	rm -rf ../MathCat
	
.PHONY: all

copy:
	./copy.sh "v${mathcatVersion}"

ios:
	$(call start);
	@echo "$${lib_static}" >>  ${cargofile}
	$(call last);

ios_local:
	$(call start_local);
	@echo "$${lib_static}" >>  ${cargofile}
	$(call last_local);


# Catalyst version still not working sadly, it gets stuck at is_dir when setting the Rules dir.
catalyst:
	$(call start);
	@echo "$${lib_static}" >>  ${cargofile}
	$(call last);
	
catalyst_local:
	$(call start_local);
	@echo "$${lib_static}" >>  ${cargofile}
	$(call last_local);
	

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
