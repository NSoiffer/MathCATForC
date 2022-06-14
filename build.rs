//! The build.rs file is necessary to unzip the rules.
//! rules.zip are needed so there is a way to get the rules dir into the build since you can't get it from the crate.

extern crate cbindgen;

use std::path::{PathBuf, Path};
use zip::ZipArchive;
use std::env;

fn main() {
    let c_examples_dir = PathBuf::from("c-example");
    unzip_rules(&c_examples_dir);
    write_headers(&c_examples_dir);
}

fn unzip_rules(location: &Path) {
    let archive = libmathcat::ZIPPED_RULE_FILES;
    let archive = std::io::Cursor::new(archive);

    let mut zip_archive = ZipArchive::new(archive).unwrap();
    zip_archive.extract(location).expect("Zip extraction failed");

    // the test dir 'zz' doesn't need to be part of the addon
    let mut zz_dir = PathBuf::from(location);
    zz_dir.push("Rules/Languages/zz");
    std::fs::remove_dir_all(&zz_dir)
        .expect(&format!("Failed to remove directory {}", zz_dir.to_str().unwrap()));
}

fn write_headers(location: &Path) {
    let crate_dir = env::var("CARGO_MANIFEST_DIR").unwrap();
    let mut location = PathBuf::from(location);
    location.push("mathcat.h");
    cbindgen::Builder::new()
    .with_crate(crate_dir)
    .generate()
    .expect("Unable to generate bindings")
    .write_to_file(location);
}   