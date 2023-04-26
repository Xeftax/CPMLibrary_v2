#include <cxxtest/TestSuite.h>
#include "cpmrequest.h"


class debugTestSuite : public CxxTest::TestSuite {

    public:

        void setUp() {
            cout << endl << ".START!" << endl;
        }
        void tearDown() {
            cout << ".END!" << endl;
        }

        void testDebug(void) {
        
        }
};