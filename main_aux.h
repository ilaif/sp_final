#ifndef MAIN_AUX_H_
#define MAIN_AUX_H_

#define BUF_SIZE 1024

extern "C" {
#include "SPLogger.h"
#include "SPConfig.h"
}

//TODO: Docs
SPConfig loadConfig(int argc, char *argv[], SP_CONFIG_MSG *msg);

//TODO: Docs
void clearConfig(SPConfig conf, SP_CONFIG_MSG *msg);

//TODO: Docs
SP_LOGGER_MSG initLogger(SPConfig conf);

//TODO: Docs
void clearLogger();

#endif /* MAIN_AUX_H_ */