// This is a simple test file to show the use of the interface calls and verify that they work in at least these cases.
//
// Compile and run this with something like
//   gcc test.cpp -o test -L. -llibmathcat_c
// assuming libmathcat_c.dll is in the current directory
//
// To run
// ./test -- prints the MathML input, the speech output (assuming no speech markup), and Nemeth braille
// test takes one optional argument. If present, repeats that input the number of times given.
// Right now, it only tells the total time and time per call, but I want to add something for
//   memory used so it can indicate a memory leak.


#include "mathcat.h"
#include <stdio.h>
#include <cstring>


// A convenient wrapper that deals with errors and memory
bool SetMathCatPreference(const char* pref, const char* value) {
    const char* status = SetPreference(pref, value);
    if (status == "") {
        const char* message = GetError();
        printf("Error setting %s preference to %s --ignoring... Message is: %s\n", pref, value, message);
        FreeMathCATString((char*)message);
        return false;
    }
    FreeMathCATString((char*)status);
    return true;
}

bool setPrefsAndMathML(const char* mathml) {
    if (!SetMathCatPreference("Language", "en")) {
        return false;
    };
    if (!SetMathCatPreference("SpeechStyle", "SimpleSpeak")) {
        return false;
    };
    if (!SetMathCatPreference("TTS", "None")) {
        return false;
    };

    const char* returnedMathML = SetMathML(mathml);
    if (returnedMathML == "") {
        const char* message = GetError();
        printf("Illegal MathML. Message is: %s\n", message);
        FreeMathCATString((char*)message);
        FreeMathCATString((char*)returnedMathML);
	    return false;
    }
    FreeMathCATString((char*)returnedMathML);
    return true;
}

bool singleTest(const char* mathml, bool doPrint) {    
    if (!setPrefsAndMathML(mathml)) {
        printf("***Error in setting prefs\n");
        return false;
    }

    const char* speech = GetSpokenText();
    const char* expected_speech = "m choose n times; open paren, the square root of m squared plus n end root; close paren";
    if (!*speech) {
        const char* message = GetError();
        printf("***Error in speech --ignoring... Message is: %s\n", message);
        FreeMathCATString((char*)message);
        return false;
    } else if (strcmp(speech, expected_speech) != 0) {
        printf("***Returned speech '%s' doesn't match expected speech '%s'\n", speech, expected_speech);
        FreeMathCATString((char*)speech);
        return false;
    } else if (doPrint) {
        printf("SimpleSpeak speech: '%s'\n", speech);
    }
    FreeMathCATString((char*)speech);

    if (!SetMathCatPreference("BrailleCode", "Nemeth")) {
        return false;
    }

    const char* braille = GetBraille("");
    const char* expected_braille = "⠷⠍⠩⠝⠾⠷⠜⠍⠘⠆⠐⠬⠝⠻⠾";
    if (!*braille) {
        const char* message = GetError();
        printf("***Error in braille --ignoring... Message is: %s\n", message);
        FreeMathCATString((char*)message);
    } else if (strcmp(braille, expected_braille) != 0) {
        printf("***Returned braille '%s' doesn't match expected braille '%s'\n", braille, expected_braille);
        FreeMathCATString((char*)braille);
        return false;
    } else if (doPrint) {
        printf("Nemeth braille: %s\n", braille);
    }
    FreeMathCATString((char*)braille);

    return true;
}

bool navGetLocationTest(NavigationLocation location, const char* expected_id, unsigned int expected_offset, bool doPrint) {
    if (!*location.id) {
        const char* message = GetError();
        printf("***Error in GetNavigationLocation... Message is: %s\n", message);
        FreeMathCATString((char*)message);
        return false;
    } else if (strcmp(location.id, expected_id) != 0) {
        printf("***GetNavigationLocation id '%s' doesn't match expected id '%s'\n", location.id, expected_id);
        FreeMathCATString((char*)location.id);
        return false;
    } else if (location.offset != expected_offset) {
        printf("***GetNavigationLocation offset '%s' doesn't match expected offset '%s'\n", location.offset, expected_offset);
        FreeMathCATString((char*)location.id);
        return false;
    } else if (doPrint) {
        printf("Location: '%s', %d\n", location.id, location.offset);
    }
    return true;
}

bool navTest(const char* mathml, bool doPrint) {
    setPrefsAndMathML(mathml);

    const char* nav_speech = DoNavigateCommand("ZoomIn");
    const char* expected_speech = "m choose n";
    if (!*nav_speech) {
        const char* message = GetError();
        printf("***Error in navigation speech... Message is: %s\n", message);
        FreeMathCATString((char*)message);
        return false;
    } else if (strcmp(nav_speech, expected_speech) != 0) {
        printf("***Returned speech '%s' doesn't match expected speech '%s'\n", nav_speech, expected_speech);
        FreeMathCATString((char*)nav_speech);
        return false;
    } else if (doPrint) {
        printf("SimpleSpeak speech: '%s'\n", nav_speech);
    }


    if (!navGetLocationTest(GetNavigationLocation(), "mrow-1", 0, doPrint)) {
        return false;
    }
    if (!navGetLocationTest(GetNavigationLocationFromBraillePosition(1), "id-m", 0, doPrint)) {
        return false;
    }

    NavigationLocation location;
    location.id = "id-m-squared";
    location.offset = 0;
    if (!*SetNavigationLocation(location)) {
        const char* message = GetError();
        printf("***Error in setting navigation location... Message is: %s\n", message);
        FreeMathCATString((char*)message);
        return false;
    }
    if (!navGetLocationTest(GetNavigationLocation(), location.id, location.offset, doPrint)) {
        return false;
    }

    if (!SetMathCatPreference("BrailleCode", "LaTeX")) {
        return false;
    }
    const char* braille = GetNavigationBraille();
    const char* expected_braille = "m^2";
    if (!*braille) {
        const char* message = GetError();
        printf("***Error in navigation braille --ignoring... Message is: %s\n", message);
        FreeMathCATString((char*)message);
    } else if (strcmp(braille, expected_braille) != 0) {
        printf("***Returned navigation braille '%s' doesn't match expected braille '%s'\n", braille, expected_braille);
        FreeMathCATString((char*)braille);
        return false;
    } else if (doPrint) {
        printf("Nemeth braille: %s\n", braille);
    }
    FreeMathCATString((char*)braille);

    return true;
}

#include <time.h>

void testForMemoryLeak(const char* mathml, int nLoops) {
    clock_t begin = clock();
    for (int i=0; i< nLoops; i++) {
        singleTest(mathml, false);
    }
    clock_t end = clock();
    double total_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Total time taken: %.2f secs; per loop %.1f ms\n", total_time, 1000 * total_time/nLoops);
}

int main(int argc, char *argv[]) {
    const char* mathml = "<math>\n\
        <mrow id='outer-mrow'>\n\
            <mrow id='mrow-1'><mo>(</mo><mfrac linethickness='0' id='choose'><mi id='id-m'>m</mi><mi>n</mi></mfrac><mo>)</mo></mrow>\n\
            <mrow id='mrow-2'><mo>(</mo><msqrt id='msqrt'><msup id='id-m-squared'><mi>m</mi><mn>2</mn></msup><mo>+</mo><mi>n</mi></msqrt><mo>)</mo></mrow>\n\
        </mrow>\n\
    </math>";

    const char* version = GetMathCATVersion();
    printf("Testing MathCAT version %s with mathml:\n%s\n", version, mathml);
    FreeMathCATString((char*)version);

    const char* rulesDirResult = SetRulesDir("./Rules");
    if (rulesDirResult == "") { // empty strings are signs of an error
        const char* message = GetError();
        printf("Did not find Rules dir. Message is: %s\n", message);
        FreeMathCATString((char*)message);
        FreeMathCATString((char*)rulesDirResult);
	    exit(1);
    }
    FreeMathCATString((char*)rulesDirResult);

    if (!singleTest(mathml, true)) {    // shows results and removes initialization times from the loop
        printf("'singleTest' returned failure\n");
        exit(1);
    }

    if (!navTest(mathml, true)) {
        printf("'navTest' returned failure\n");
        exit(1);
    }

    if (argc > 1) {    // argv[0] is command
        printf("Looping %d times...\n", atoi(argv[1]));
        testForMemoryLeak(mathml, atoi(argv[1])-1); // already did once
    }
    
    printf("Success!\n");
    return EXIT_SUCCESS;
}
