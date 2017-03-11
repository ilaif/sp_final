#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "unit_test_util.h" //SUPPORTING MACROS ASSERT_TRUE/ASSERT_FALSE etc..
#include "SPConfig.h"

//Logger is not defined
static bool basicConfigTest() {
    SP_CONFIG_MSG *msg = (SP_CONFIG_MSG *) malloc(sizeof(msg));
    SPConfig conf = spConfigCreate("/Users/ilaif/uni/software-project/final/basicConfigTest.cnf", msg);
    if (conf == NULL) {
        free(msg);
        return false;
    }
    ASSERT_TRUE(*msg == SP_CONFIG_MISSING_DIR);
    free(msg);
    return true;
}

//TODO: Write more unit tests

int main() {
    RUN_TEST(basicConfigTest);
    return 0;
}
