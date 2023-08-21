#include <cstdarg>
#include <cstdint>
#include <cstdlib>
#include <ostream>
#include <new>

extern "C" {

/// Returns the error set by the last call.
/// Calling GetError() will clear the current error.
/// If there is no error, "" (an empty string) will be returned.
const char *GetError();

/// IMPORTANT: For every MathCAT function that returns a string, it must be free'd with this call
/// If this is not called, the memory will be leaked.
void FreeMathCATString(char *str);

/// The absolute path location of the MathCAT Rules dir.
/// IMPORTANT: This should be the first call to MathCAT
const char *SetRulesDir(const char *rules_dir_location);

/// The MathML to be spoken, brailled, or navigated.
///
/// This will override any previous MathML that was set.
/// Returns: the MathML that was set, annotated with 'id' values on each node (if none were present)
/// The 'id' values can be used during navigation for highlighting the current node
const char *SetMathML(const char *mathml_str);

/// Get the spoken text of the MathML that was set.
/// The speech takes into account any AT or user preferences.
const char *GetMathCATVersion();

/// Get the spoken text of the MathML that was set.
/// The speech takes into account any AT or user preferences.
const char *GetSpokenText();

/// Set an API preference. The preference name should be a known preference name.
/// The value should either be a string or a number (depending upon the preference being set)
///
/// This function can be called multiple times to set different values.
/// The values are persistent but can be overwritten by setting a preference with the same name and a different value.
const char *SetPreference(const char *name,
                          const char *value);

/// Set an API preference. The preference name should be a known preference name.
/// The value should either be a string or a number (depending upon the preference being set)
///
/// This function can be called multiple times to set different values.
/// The values are persistent but can be overwritten by setting a preference with the same name and a different value.
const char *GetPreference(const char *name);

/// Get the braille associated with the MathML node with a given id (MathML set by `SetMathML`]).
/// An empty string can be used to return the braille associated with the entire expression.
///
/// The braille returned depends upon the preference for braille output.
const char *GetBraille(const char *nav_node_id);

/// Given a key code along with the modifier keys, the current node is moved accordingly (or value reported in some cases).
///
/// The spoken text for the new current node is returned.
const char *DoNavigateKeyPress(uintptr_t key,
                               bool shift_key,
                               bool control_key,
                               bool alt_key,
                               bool meta_key);

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
const char *DoNavigateCommand(const char *command);

/// Return the MathML associated with the current (navigation) node.
const char *GetNavigationMathMLId();

/// Return the MathML associated with the current (navigation) node.
const char *GetNavigationMathML();

} // extern "C"
