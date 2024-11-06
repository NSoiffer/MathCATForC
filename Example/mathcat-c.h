#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/// `NavigationLocation` is a structure used with Navigation.
/// In many cases, the `id` is enough to uniquely identify the navigation location.
/// However, for a number such as "123" or an identifier such as "sin", there is no `id` representing each character.
/// An `offset` is used to uniquely identify each character. `offset` = 0 is the entire identifier, 1 is the first char, etc.
/// For example, the "i" in `<mi id='xyz-123'>sin</mi>` has `id="xyz-123"` and `offset=1`.
///
/// Note: currently (2/24) offsets are not implemented in MathCAT and will always return 0. This will hopefully be supported by the end of 2024.
typedef struct NavigationLocation {
  const char *id;
  uint32_t offset;
} NavigationLocation;

/// Returns the error set by the last call.
/// Calling GetError() will clear the current error.
/// If there is no error, "" (an empty string) will be returned.
const char *GetError(void);

/// IMPORTANT: For every MathCAT function that returns a string, it must be free'd with this call
/// If this is not called, the memory will be leaked.
void FreeMathCATString(char *str);

/// The absolute path location of the MathCAT Rules dir.
/// Returns "Ok" or an empty string if there is an error (use GetError()).
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
const char *GetMathCATVersion(void);

/// Get the spoken text of the MathML that was set.
/// The speech takes into account any AT or user preferences.
const char *GetSpokenText(void);

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

/// Get the braille associated with the MathML that was set by [`set_mathml`].
/// The braille returned depends upon the preference for the `code` preference (default `Nemeth`).
///
/// If 'nav_node_id' is a non-empty string, the node is highlighted based on the value of `BrailleNavHighlight` (default: `EndPoints`)
const char *GetBraille(const char *nav_node_id);

/// Get the braille associated with the current navigation focus of the MathML that was set by [`set_mathml`].
/// The braille returned depends upon the preference for the `code` preference (default `Nemeth`).
///
/// The returned braille is brailled as if the current navigation focus is the entire expression to be brailled.
const char *GetNavigationBraille(void);

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
const char *GetNavigationMathML(void);

/// Return the id of the MathML associated with the current (navigation) node.
/// Note: this is deprecated -- use GetNavigationLocation()
const char *GetNavigationMathMLId(void);

/// Return the offset from the MathML node associated with the current (navigation) node.
/// Note: this is deprecated -- use GetNavigationLocation()
uint32_t GetNavigationMathMLOffset(void);

/// Set the location of the navigation node associated with the current MathML expression.
/// Returns "Ok" or an empty string if there is an error (use GetError()).
const char *SetNavigationLocation(struct NavigationLocation location);

/// Return the NavigationLocation (id and offset) associated with the current (navigation) node.
/// If there is an error, the id is set to an empty string (use GetError()).
struct NavigationLocation GetNavigationLocation(void);

/// Return the NavigationLocation (id and offset) associated with braille cursor location (0-based).
/// If there is an error, the id is set to an empty string (use GetError()).
struct NavigationLocation GetNavigationLocationFromBraillePosition(uint32_t position);
