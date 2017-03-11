#include <cstring>
#include <cstdlib>
#include "main_aux.h"

#define INVALID_COMMAND_LINE "Invalid command line : use -c <config_filename>\n"
#define DEFAULT_CONF_CANNOT_OPEN "The default configuration file spcbir.config couldn’t be open\n"
#define CONF_CANNOT_OPEN "The configuration file %s couldn’t be open\n"

SPConfig loadConfig(int argc, char *argv[], SP_CONFIG_MSG *msg) {
    SPConfig conf = NULL;
    bool load_default_config = false;

    if (argc < 3) {
        load_default_config = true;
    } else {
        if (strcmp(argv[1], "-c") != 0) {
            printf(INVALID_COMMAND_LINE);
            load_default_config = true;
        }
    }

    conf = spConfigCreate((load_default_config) ? "./spcbir.config" : argv[2], msg);
    if (*msg == SP_CONFIG_CANNOT_OPEN_FILE) { // If cannot open file, then open the default one.
        if (load_default_config) {
            printf(DEFAULT_CONF_CANNOT_OPEN);
        } else {
            printf(CONF_CANNOT_OPEN, argv[2]);
            conf = spConfigCreate("./spcbir.config", msg);
            if (*msg == SP_CONFIG_CANNOT_OPEN_FILE) {
                printf(DEFAULT_CONF_CANNOT_OPEN);
            }
        }
    }

    return conf;
}

void clearConfig(SPConfig conf, SP_CONFIG_MSG *msg) {
    free(conf);
    free(msg);
}

SP_LOGGER_MSG initLogger(SPConfig conf) {
    char filename[50];
    spConfigGetLoggerFilename(filename, conf);
    SP_LOGGER_LEVEL level;
    spConfigGetLoggerLevel(&level, conf);
    SP_LOGGER_MSG logger_msg = spLoggerCreate(filename, level);
    return logger_msg;
}

void clearLogger() {
    spLoggerDestroy();
}