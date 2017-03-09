#include "SPLogger.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//File open mode
#define SP_LOGGER_OPEN_MODE "w"

// Global variable holding the logger
SPLogger logger = NULL;

struct sp_logger_t {
    FILE *outputChannel; //The logger file
    bool isStdOut; //Indicates if the logger is stdout
    SP_LOGGER_LEVEL level; //Indicates the level
};

SP_LOGGER_MSG spLoggerCreate(const char *filename, SP_LOGGER_LEVEL level) {
    if (logger != NULL) { //Already defined
        return SP_LOGGER_DEFINED;
    }
    logger = (SPLogger) malloc(sizeof(*logger));
    if (logger == NULL) { //Allocation failure
        return SP_LOGGER_OUT_OF_MEMORY;
    }
    logger->level = level; //Set the level of the logger
    if (filename == NULL) { //In case the filename is not set use stdout
        logger->outputChannel = stdout;
        logger->isStdOut = true;
    } else { //Otherwise open the file in write mode
        logger->outputChannel = fopen(filename, SP_LOGGER_OPEN_MODE);
        if (logger->outputChannel == NULL) { //Open failed
            free(logger);
            logger = NULL;
            return SP_LOGGER_CANNOT_OPEN_FILE;
        }
        logger->isStdOut = false;
    }
    return SP_LOGGER_SUCCESS;
}

/**
 * Helper function to print error, warning and info logs
 * @param level
 * @param msg
 * @param file
 * @param function
 * @param line
 * @return
 */
SP_LOGGER_MSG spLoggerPrintMessage(const char *level, const char *msg, const char *file,
                                   const char *function, const int line) {
    int res = 0;
    if (logger->isStdOut) {
        printf("---%s---\n", level);
        printf("- file: %s\n", file);
        printf("- function: %s\n", function);
        printf("- line: %d\n", line);
        printf("- message: %s\n", msg);
    } else {
        res = fprintf(logger->outputChannel, "---%s---\n", level);
        if (!res) return SP_LOGGER_WRITE_FAIL;
        res = fprintf(logger->outputChannel, "- file: %s\n", file);
        if (!res) return SP_LOGGER_WRITE_FAIL;
        res = fprintf(logger->outputChannel, "- function: %s\n", function);
        if (!res) return SP_LOGGER_WRITE_FAIL;
        res = fprintf(logger->outputChannel, "- line: %d\n", line);
        if (!res) return SP_LOGGER_WRITE_FAIL;
        res = fprintf(logger->outputChannel, "- message: %s\n", msg);
        if (!res) return SP_LOGGER_WRITE_FAIL;
    }

    return SP_LOGGER_SUCCESS;
}

SP_LOGGER_MSG spLoggerPrintError(const char *msg, const char *file, const char *function, const int line) {
    if (logger == NULL) { //Already defined
        return SP_LOGGER_UNDIFINED;
    }
    SP_LOGGER_MSG res = SP_LOGGER_UNDIFINED;
    if (logger->level >= SP_LOGGER_ERROR_LEVEL) {
        res = spLoggerPrintMessage("ERROR", msg, file, function, line);
    } else {
        res = SP_LOGGER_SUCCESS;
    }
    return res;
}

SP_LOGGER_MSG spLoggerPrintWarning(const char *msg, const char *file, const char *function, const int line) {
    if (logger == NULL) { //Already defined
        return SP_LOGGER_UNDIFINED;
    }
    SP_LOGGER_MSG res = SP_LOGGER_UNDIFINED;
    if (logger->level >= SP_LOGGER_WARNING_ERROR_LEVEL) {
        res = spLoggerPrintMessage("WARNING", msg, file, function, line);
    } else {
        res = SP_LOGGER_SUCCESS;
    }
    return res;
}

SP_LOGGER_MSG spLoggerPrintInfo(const char *msg) {
    if (logger == NULL) {
        return SP_LOGGER_UNDIFINED;
    }
    if (logger->level >= SP_LOGGER_INFO_WARNING_ERROR_LEVEL) {
        int print_res = 0;
        if (logger->isStdOut) {
            printf("---INFO---\n");
            printf("- message: %s\n", msg);
        } else {
            print_res = fprintf(logger->outputChannel, "---INFO---\n");
            if (print_res < 0) return SP_LOGGER_WRITE_FAIL;
            print_res = fprintf(logger->outputChannel, "- message: %s\n", msg);
            if (print_res < 0) return SP_LOGGER_WRITE_FAIL;
        }
    }
    return SP_LOGGER_SUCCESS;
}

SP_LOGGER_MSG spLoggerPrintDebug(const char *msg, const char *file, const char *function, const int line) {
    if (logger == NULL) { //Already defined
        return SP_LOGGER_UNDIFINED;
    }
    SP_LOGGER_MSG res = SP_LOGGER_UNDIFINED;
    if (logger->level >= SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL) {
        res = spLoggerPrintMessage("DEBUG", msg, file, function, line);
    } else {
        res = SP_LOGGER_SUCCESS;
    }
    return res;
}


SP_LOGGER_MSG spLoggerPrintMsg(const char *msg) {
    if (logger == NULL) {
        return SP_LOGGER_UNDIFINED;
    }
    if (logger->level >= SP_LOGGER_INFO_WARNING_ERROR_LEVEL) {
        int print_res = 0;
        if (logger->isStdOut) {
            printf("%s\n", msg);
        } else {
            print_res = fprintf(logger->outputChannel, "%s\n", msg);
            if (print_res < 0) return SP_LOGGER_WRITE_FAIL;
        }
    }
    return SP_LOGGER_SUCCESS;
}

void spRegularMessage(const char *msg, const char *file, const int line) {
    printf("File: %s\n", file);
    printf("Line: %d\n", line);
    printf("Message: %s\n", msg);
}

void spLoggerDestroy() {
    if (!logger) {
        return;
    }
    if (!logger->isStdOut) {//Close file only if not stdout
        fclose(logger->outputChannel);
    }
    free(logger);//free allocation
    logger = NULL;
}
