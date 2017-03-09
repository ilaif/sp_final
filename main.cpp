#include <iostream>

extern "C" {
#include "SPLogger.h"
#include "SPConfig.h"
}

#define INVALID_COMMAND_LINE "Invalid command line : use -c <config_filename>\n"
#define DEFAULT_CONF_CANNOT_OPEN "The default configuration file spcbir.config couldn’t be open\n"
#define CONF_CANNOT_OPEN "The configuration file %s couldn’t be open\n"

int main(int argc, char *argv[]) {

    SP_CONFIG_MSG *msg = (SP_CONFIG_MSG *) malloc(sizeof(msg));
    SPConfig conf = NULL;
    if (argc < 3 || strcmp(argv[1], "-c") != 0) {
        printf(INVALID_COMMAND_LINE);
        // Default config file in case of a problem
        conf = spConfigCreate("spcbir.config", msg);
        if (*msg == SP_CONFIG_CANNOT_OPEN_FILE) {
            printf(DEFAULT_CONF_CANNOT_OPEN);
        }
    } else {
        conf = spConfigCreate(argv[2], msg);
        if (*msg == SP_CONFIG_CANNOT_OPEN_FILE) { // If cannot open file, then open the default one.
            printf(CONF_CANNOT_OPEN, argv[2]);
            conf = spConfigCreate("spcbir.config", msg);
            if (*msg == SP_CONFIG_CANNOT_OPEN_FILE) {
                printf(DEFAULT_CONF_CANNOT_OPEN);
            }
        }
    }

    //TODO:
    /*SP_LOGGER_MSG lm = spLoggerCreate(, SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL);

    free(msg);

    if (lm != SP_LOGGER_SUCCESS) {

    }

    spLoggerPrintInfo("Hey");*/


    return 0;
}