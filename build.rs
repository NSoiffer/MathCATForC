//! The build.rs file is necessary to unzip the rules.
//! rules.zip are needed so there is a way to get the rules dir into the build since you can't get it from the crate.

extern crate cbindgen;

use std::path::{PathBuf, Path};
use std::env;
use cbindgen::*;


#[cfg(feature = "include-zip")]
use zip::ZipArchive;
#[cfg(feature = "include-zip")]
fn unzip_rules(location: &Path) {
        let archive = libmathcat::ZIPPED_RULE_FILES;
        let archive = std::io::Cursor::new(archive);

        let mut zip_archive = ZipArchive::new(archive).unwrap();
        zip_archive.extract(location).expect("Zip extraction failed");
}

fn main() {
    let examples_dir = PathBuf::from("Example");

    #[cfg(feature = "include-zip")]
    unzip_rules(&examples_dir);
    
    write_headers(&examples_dir, "mathcat-c.h", Language::C);
    write_headers(&examples_dir, "mathcat.h", Language::Cxx);
}

fn write_headers(location: &Path, name: &str, header_style: cbindgen::Language) {
    let config = cbindgen::Config::from_file(
            if header_style == Language::Cxx {"cbindgen.toml"} else {"cbindgen-c.toml"}
        ).expect("couldn't read cbindgen.toml");
    let crate_dir = env::var("CARGO_MANIFEST_DIR").unwrap();
    let mut location = PathBuf::from(location);
    location.push(name);
    cbindgen::Builder::new()
    .with_language(header_style)
    .with_cpp_compat(true)
    .with_config(config)
    .with_crate(crate_dir)
    .generate()
    .expect("Unable to generate bindings")
    .write_to_file(location);
}   