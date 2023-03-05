//! When calling from C, the general ordering is:
//! 1. whatever preferences the AT needs to set, it is done with calls to [`SetPreference`].
//! 2. the MathML is sent over via [`SetMathML`].
//! 3. AT calls to get the speech [`GetSpokenText`] and calls [`GetBraille`] to get the (Unicode) braille.
//! 
//! Navigation can be done via calls to either:
//! * [`DoNavigateKeyPress`] (takes key events as input)
//! * [`DoNavigateCommand`] (takes the commands the key events internally map to)
//! Both return a string to speak.
//! To highlight the node on is on, 'id's are used. If they weren't already present,
//! [`SetMathML`] returns a string representing MathML that contains 'id's for any node that doesn't already
//! have an 'id' set. You can get the current node with
//! * [`GetNavigationMathMLId`]
//! * [`GetNavigationMathML`] -- returns a string representing the MathML for the selected node
//! Note: a second integer is returned. This is the offset in characters for a leaf node.
//!   This is needed when navigating by character for multi-symbol leaf nodes such as "sin" and "1234"
//! 
//! It is also possible to find out what preferences are currently set by calling [`GetPreference`]
//!
//! AT can pass key strokes to allow a user to navigate the MathML by calling [`DoNavigateKeyPress`]; the speech is returned.
//! To get the MathML associated with the current navigation node, call [`GetNavigationMathML`].
//!
//! Internally, Rust returns a Result<> type. I don't know how to express this in the interface (maybe it is possible via enums?).
//! Instead, most functions return a string. When the string is empty, that indicates an error.
//! For functions that one doesn't normally expect a string (e.g, [`SetRulesDir`]), "Ok" is returned for consistency.
//! In that case, call [`GetError`] and it returns a string representing the error.
//! For the two functions that return integers ([`GetNavigationMathMLIdOffset`] and [`GetNavigationMathMLOffset`]),
//!   a negative value indicates an error.
//! 
//! IMPORTANT: Rust allocates all the strings and its allocator and C's allocator and not compatible.
//!   EVERY string returned from Rust (including errors/empty strings) must be deallocated by calling FreeMathCATString().
//! Failure to call FreeMathCATString() will result in a memory leak.

#![allow(non_snake_case)]
#![allow(clippy::needless_return)]

use libmathcat::*;
use std::cell::RefCell;
use std::ffi::{CString, CStr};
use std::os::raw::c_char;

const ILLEGAL_STRING: &str = "Internal MathCAT error -- internal null";
// const ILLEGAL_CStr: &str = "Illegal bytes in UTF8 input string";
thread_local!{
    /// The current node being navigated (also spoken and brailled) is stored in `MATHML_INSTANCE`.
    pub static ERROR_STRING: RefCell<String> = RefCell::new(String::default());
}

fn change_error_string_value(new_string: String) {
    ERROR_STRING.with(|string: &RefCell<String>| {
        let mut string = string.borrow_mut();
        string.clear();
        string.push_str(&new_string);
    })
}

#[no_mangle]
/// Returns the error set by the last call.
/// Calling GetError() will clear the current error.
/// If there is no error, "" (an empty string) will be returned.
pub extern "C" fn GetError() -> *const c_char {
    ERROR_STRING.with(|string: &RefCell<String>| {
        let mut string = string.borrow_mut();
        let result = CString::new(string.as_bytes()).expect(ILLEGAL_STRING);
        string.clear();
        return result.into_raw();
    })
}



fn safe_string(cstr: *const c_char) -> String {
    return unsafe {
        CStr::from_ptr(cstr).to_string_lossy().into_owned()
    }
}

/// The error type returned from MathCAT is from an error package, so we can't implement From on it.
/// Instead, we write a wrapper function that deals with the error conversion
fn set_string_error(result: Result<String, libmathcat::errors::Error>) -> *const c_char {
    let result =  match result {
        Ok(answer) => CString::new(answer).expect(ILLEGAL_STRING),
        Err(e) => {
            change_error_string_value(errors_to_string(&e));
            CString::new("").unwrap()
        },
    };
    return result.into_raw();
}

fn set_int_error(result: Result<usize, libmathcat::errors::Error>) -> i32 {
    return match result {
        Ok(answer) => answer as i32,
        Err(e) => {
            change_error_string_value(errors_to_string(&e));
            -1
        },
    };
}

fn set_empty_error(result: Result<(), libmathcat::errors::Error>) -> *const c_char {
    let result = match result {
        Ok(_) => CString::new("Ok").expect(ILLEGAL_STRING),
        Err(e) => {
            change_error_string_value(errors_to_string(&e));
            CString::new("").unwrap()
        },
    };
    return result.into_raw();
}

#[no_mangle]
/// IMPORTANT: For every MathCAT function that returns a string, it must be free'd with this call
/// If this is not called, the memory will be leaked.
pub extern "C" fn FreeMathCATString(str: *mut c_char) {
    unsafe{ CString::from_raw(str) };
}

#[no_mangle]
/// The absolute path location of the MathCAT Rules dir.
/// IMPORTANT: This should be the first call to MathCAT
pub extern "C" fn SetRulesDir(rules_dir_location: *const c_char) -> *const c_char {
    return set_empty_error(
        set_rules_dir(safe_string(rules_dir_location))
    );
}

#[no_mangle]
/// The MathML to be spoken, brailled, or navigated.
///
/// This will override any previous MathML that was set.
/// Returns: the MathML that was set, annotated with 'id' values on each node (if none were present)
/// The 'id' values can be used during navigation for highlighting the current node
pub extern "C" fn SetMathML(mathml_str: *const c_char) -> *const c_char {
    return set_string_error(
        set_mathml(safe_string(mathml_str))
    );
}

#[no_mangle]
/// Get the spoken text of the MathML that was set.
/// The speech takes into account any AT or user preferences.
pub extern "C" fn GetMathCATVersion() -> *const c_char {    
    return set_string_error( Ok(get_version()) );
}

#[no_mangle]
/// Get the spoken text of the MathML that was set.
/// The speech takes into account any AT or user preferences.
pub extern "C" fn GetSpokenText() -> *const c_char {
    return set_string_error( get_spoken_text() );
}

#[no_mangle]
/// Set an API preference. The preference name should be a known preference name.
/// The value should either be a string or a number (depending upon the preference being set)
///
/// This function can be called multiple times to set different values.
/// The values are persistent but can be overwritten by setting a preference with the same name and a different value.
pub extern "C" fn SetPreference(name: *const c_char, value: *const c_char) -> *const c_char {
    let name = safe_string(name);
    let value = safe_string(value);
    return set_empty_error( set_preference(name, value) );
}

#[no_mangle]
/// Set an API preference. The preference name should be a known preference name.
/// The value should either be a string or a number (depending upon the preference being set)
///
/// This function can be called multiple times to set different values.
/// The values are persistent but can be overwritten by setting a preference with the same name and a different value.
pub extern "C" fn GetPreference(name: *const c_char) -> *const c_char {
    return match get_preference( safe_string(name) ) {
        Ok(value) => {
            set_string_error( Ok(value))
        },
        Err(_) => {
            change_error_string_value( format!("Unknown preference name {}", safe_string(name)) );
            set_string_error( Ok( String::default() ) )
        },
    };
}

#[no_mangle]
#[allow(unused_variables)]
/// Get the braille associated with the MathML node with a given id (MathML set by `SetMathML`]).
/// An empty string can be used to return the braille associated with the entire expression.
/// 
/// The braille returned depends upon the preference for braille output.
pub extern "C" fn GetBraille(nav_node_id: *const c_char) -> *const c_char {
    return set_string_error( get_braille(safe_string(nav_node_id)) );
}

#[no_mangle]
/// Given a key code along with the modifier keys, the current node is moved accordingly (or value reported in some cases).
///
/// The spoken text for the new current node is returned.
pub extern "C" fn DoNavigateKeyPress(key: usize, shift_key: bool, control_key: bool, alt_key: bool, meta_key: bool) -> *const c_char {
    return set_string_error( do_navigate_keypress(key, shift_key, control_key, alt_key, meta_key) );
}

#[no_mangle]
/// Given a command, the current node is moved accordingly (or value reported in some cases).
///
/// The spoken text for the new current node is returned.
/// 
/// The list of legal commands are:
/// "MovePrevious", "MoveNext", "MoveStart", "MoveEnd", "MoveLineStart", "MoveLineEnd", 
/// "MoveCellPrevious", "MoveCellNext", "MoveCellUp", "MoveCellDown", "MoveColumnStart", "MoveColumnEnd", 
/// "ZoomIn", "ZoomOut", "ZoomOutAll", "ZoomInAll", 
/// "MoveLastLocation", 
/// "ReadPrevious", "ReadNext", "ReadCurrent", "ReadCellCurrent", "ReadStart", "ReadEnd", "ReadLineStart", "ReadLineEnd", 
/// "DescribePrevious", "DescribeNext", "DescribeCurrent", 
/// "WhereAmI", "WhereAmIAll", 
/// "ToggleZoomLockUp", "ToggleZoomLockDown", "ToggleSpeakMode", 
/// "Exit", 
/// "MoveTo0","MoveTo1","MoveTo2","MoveTo3","MoveTo4","MoveTo5","MoveTo6","MoveTo7","MoveTo8","MoveTo9",
/// "Read0","Read1","Read2","Read3","Read4","Read5","Read6","Read7","Read8","Read9",
/// "Describe0","Describe1","Describe2","Describe3","Describe4","Describe5","Describe6","Describe7","Describe8","Describe9",
/// "SetPlacemarker0","SetPlacemarker1","SetPlacemarker2","SetPlacemarker3","SetPlacemarker4","SetPlacemarker5","SetPlacemarker6","SetPlacemarker7","SetPlacemarker8","SetPlacemarker9",
pub extern "C" fn DoNavigateCommand(command: *const c_char) -> *const c_char {
    return set_string_error( do_navigate_command(safe_string(command)) );
}

#[no_mangle]
/// Return the MathML associated with the current (navigation) node.
pub extern "C" fn GetNavigationMathMLId() -> *const c_char {
    return match get_navigation_mathml_id() {
        Err(e) => set_string_error(Err(e)),
        Ok((nav_id, _)) => set_string_error( Ok(nav_id) ),
    }
}

#[no_mangle]
/// Return the MathML associated with the current (navigation) node.
pub extern "C" fn GetNavigationMathML() -> *const c_char {
    return match get_navigation_mathml() {
        Err(e) => set_string_error(Err(e)),
        Ok((nav_mathml, _)) => set_string_error( Ok(nav_mathml) ),
    }
}

/// Return the MathML associated with the current (navigation) node.
pub extern "C" fn GetNavigationMathMLIdOffset() -> i32 {
    return match get_navigation_mathml_id() {
        Err(e) => set_int_error(Err(e)),
        Ok((_, nav_offset)) => set_int_error( Ok(nav_offset) ),
    }
}

#[no_mangle]
/// Return the MathML associated with the current (navigation) node.
pub extern "C" fn GetNavigationMathMLOffset() -> i32 {
    return match get_navigation_mathml() {
        Err(e) => set_int_error(Err(e)),
        Ok((_, nav_offset)) => set_int_error( Ok(nav_offset) ),
    }
}


// #[cfg(test)]
// mod py_tests {
//     use super::*;

//     #[test]
//     fn test_setting() {
//         // this isn't a real test
//         pyo3::prepare_freethreaded_python();
//         let mathml_str = "<math><mo>(</mo><mrow><mn>451</mn><mo>,</mo><mn>231</mn></mrow><mo>)</mo></math>";
//         match set_error( libmathcat::interface::set_mathml(mathml_str.to_string()) ) {
//             Ok(_mathml_with_ids) => println!("MathML is set w/o error"),
//             Err(e) => println!("Error is {}", e.to_string()),
//         }
//         // still alive?
//         match set_error( libmathcat::interface::set_mathml(mathml_str.to_string()) ) {
//             Ok(_mathml_with_ids) => panic!("MathML is set 2nd time w/o error"),
//             Err(e) => panic!("Error remains {}", e.to_string()),
//         }
//     }
// }
