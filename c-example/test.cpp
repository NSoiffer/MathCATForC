// Compile and run this with something like
//   gcc test.cpp -o test -L../target/release/ -llibmathcat_c
//   LD_LIBRARY_PATH="../target/release/" ./test
// where "../target/release" is the location of libmathcat_c.dll
// Compiling seems a bit touchy -- sometimes I needed to copy libmathcat_c.dll to the current dir


#include "mathcat.h"
#include <stdio.h>

bool singleTest(const char* mathml, bool doPrint) {
    const char* returnedMathML = SetMathML("<math><mo>(</mo><mfrac linethickness='0'><mi>m</mi><mi>n</mi></mfrac><mo>)</mo></math>");
    if (returnedMathML == "") {
        const char* message = GetError();
        printf("Illegal MathML. Message is: %s\n", message);
        FreeMathCATString((char*)message);
        FreeMathCATString((char*)returnedMathML);
	    return false;
    }
    FreeMathCATString((char*)returnedMathML);

    const char* status = SetPreference("SpeechStyle", "SimpleSpeak");
    if (status == "") {
        const char* message = GetError();
        printf("Error setting SpeechStyle preference --ignoring... Message is: %s\n", message);
        FreeMathCATString((char*)message);
    }
    FreeMathCATString((char*)status);
    
    const char* speech = GetSpokenText();
    if (speech == "") {
        const char* message = GetError();
        printf("Error in speech --ignoring... Message is: %s\n", message);
        FreeMathCATString((char*)message);
    } else if (doPrint) {
        printf("MathCAT speech: %s\n", speech);
    }
    FreeMathCATString((char*)speech);

    status = SetPreference("BrailleCode", "Nemeth");
    if (status == "") {
        const char* message = GetError();
        printf("Error setting BrailleCode preference --ignoring... Message is: %s\n", message);
        FreeMathCATString((char*)message);
    }
    FreeMathCATString((char*)status);

    const char* braille = GetBraille("");
    if (braille == "") {
        const char* message = GetError();
        printf("Error in braille --ignoring... Message is: %s\n", message);
        FreeMathCATString((char*)message);
    } else if (doPrint) {
        printf("MathCAT braille: %s\n", braille);
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
      <mo>(</mo><mfrac linethickness='0'><mi>m</mi><mi>n</mi></mfrac><mo>)</mo>\n\
      <mo>(</mo><msqrt><msup><mi>m</mi><mn>2</mn></msup><mo>+</mo><mi>n</mi></msqrt><mo>)</mo>\n\
    </math>";
    printf("Testing with mathml:\n%s\n", mathml);

    const char* rulesDirResult = SetRulesDir("./Rules");
    if (rulesDirResult == "") { // empty strings are signs of an error
        const char* message = GetError();
        printf("Did not find Rules dir. Message is: %s\n", message);
        FreeMathCATString((char*)message);
        FreeMathCATString((char*)rulesDirResult);
	    exit(1);
    }
    FreeMathCATString((char*)rulesDirResult);

    singleTest(mathml, true);   // shows results and removes initialization times from the loop

    if (argc > 1) {    // argv[0] is command
        printf("Looping %d times...\n", atoi(argv[1]));
        testForMemoryLeak(mathml, atoi(argv[1]));
    }
    
    return EXIT_SUCCESS;
}
