#include <iostream>

extern "C" {
#include "SPLogger.h"
}

int main() {

    SP_LOGGER_MSG lm = spLoggerCreate("log.txt", SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL);

    if (lm != SP_LOGGER_SUCCESS ) {

    }

    spLoggerPrintInfo("Hey");

    return 0;
}