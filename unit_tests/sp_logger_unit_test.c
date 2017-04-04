#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "unit_test_util.h"
#include "../SPLogger.h"


// This is a helper function which checks if two files are identical
static bool identicalFiles(const char *fname1, const char *fname2) {
    FILE *fp1, *fp2;
    fp1 = fopen(fname1, "r");
    fp2 = fopen(fname2, "r");
    char ch1 = EOF, ch2 = EOF;

    if (fp1 == NULL) {
        return false;
    } else if (fp2 == NULL) {
        fclose(fp1);
        return false;
    } else {
        ch1 = getc(fp1);
        ch2 = getc(fp2);

        while ((ch1 != EOF) && (ch2 != EOF) && (ch1 == ch2)) {
            ch1 = getc(fp1);
            ch2 = getc(fp2);
        }
        fclose(fp1);
        fclose(fp2);
    }
    if (ch1 == ch2) {
        return true;
    } else {
        return false;
    }
}

//Logger is not defined
static bool basicLoggerTest() {
    ASSERT_TRUE(spLoggerPrintError("A", "sp_logger_unit_test.c", __func__, __LINE__) == SP_LOGGER_UNDIFINED);
    spLoggerDestroy();
    return true;
}

//Only Errors should be printed
static bool basicLoggerErrorTest() {
    const char *expectedFile = "C:/Users/Noa Biton/Desktop/pro/sp_final/unit_tests/basicLoggerErrorTestExp.log";
    const char *testFile = "C:/Users/Noa Biton/Desktop/pro/sp_final/unit_tests/basicLoggerErrorTest.log";
    ASSERT_TRUE(spLoggerCreate(testFile, SP_LOGGER_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
    ASSERT_TRUE(spLoggerPrintError("MSGA", "sp_logger_unit_test.c", __func__, __LINE__) == SP_LOGGER_SUCCESS);
    ASSERT_TRUE(spLoggerPrintWarning("MSGB", "sp_logger_unit_test.c", __func__, __LINE__) == SP_LOGGER_SUCCESS);
    ASSERT_TRUE(spLoggerPrintInfo("MSGC") == SP_LOGGER_SUCCESS);
    ASSERT_TRUE(spLoggerPrintDebug("MSGD", "sp_logger_unit_test.c", __func__, __LINE__) == SP_LOGGER_SUCCESS);
    spLoggerDestroy();
    ASSERT_TRUE(identicalFiles(testFile, expectedFile));
    return true;
}

//All messages should be printed in debug level
static bool basicLoggerDebugTest() {
    const char *expectedFile = "basicLoggerDebugTestExp.log";
    const char *testFile = "basicLoggerDebugTest.log";
    ASSERT_TRUE(spLoggerCreate(testFile, SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
    ASSERT_TRUE(spLoggerPrintError("MSGA", "sp_logger_unit_test.c", __func__, __LINE__) == SP_LOGGER_SUCCESS);
    ASSERT_TRUE(spLoggerPrintWarning("MSGB", "sp_logger_unit_test.c", __func__, __LINE__) == SP_LOGGER_SUCCESS);
    ASSERT_TRUE(spLoggerPrintInfo("MSGC") == SP_LOGGER_SUCCESS);
    ASSERT_TRUE(spLoggerPrintDebug("MSGD", "sp_logger_unit_test.c", __func__, __LINE__) == SP_LOGGER_SUCCESS);
    spLoggerDestroy();
    ASSERT_TRUE(identicalFiles(testFile, expectedFile));
    return true;
}

//Error and Warning should be printed in warning level+msg
static bool basicLoggerWarningTest() {
    const char* expectedFile = "basicLoggerWarningTestExp.log";
    const char* testFile = "basicLoggerWarningTest.log";
    ASSERT_TRUE(spLoggerCreate(testFile,SP_LOGGER_WARNING_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
    ASSERT_TRUE(spLoggerPrintError("MSGA","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
    ASSERT_TRUE(spLoggerPrintWarning("MSGB","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
    ASSERT_TRUE(spLoggerPrintError("MSGA","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
    ASSERT_TRUE(spLoggerPrintDebug("MSGD","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
    ASSERT_TRUE(spLoggerPrintWarning("MSGB","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
    ASSERT_TRUE(spLoggerPrintInfo("MSGC") == SP_LOGGER_SUCCESS);
    ASSERT_TRUE(spLoggerPrintDebug("MSGD","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
    ASSERT_TRUE(spLoggerPrintMsg("MSG")==SP_LOGGER_SUCCESS);
    spLoggerDestroy();
    ASSERT_TRUE(identicalFiles(testFile,expectedFile));
    return true;
}
//Error and Warning and info should be printed in info level+msg
static bool basicLoggerInfoTest() {
    const char* expectedFile = "basicLoggerInfoTestExp.log";
    const char* testFile = "basicLoggerInfoTest.log";
    ASSERT_TRUE(spLoggerCreate(testFile,SP_LOGGER_INFO_WARNING_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
    ASSERT_TRUE(spLoggerPrintError("MSGA","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
    ASSERT_TRUE(spLoggerPrintWarning("MSGB","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
    ASSERT_TRUE(spLoggerPrintError("MSGA","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
    ASSERT_TRUE(spLoggerPrintDebug("MSGD","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
    ASSERT_TRUE(spLoggerPrintWarning("MSGB","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
    ASSERT_TRUE(spLoggerPrintInfo("MSGC") == SP_LOGGER_SUCCESS);
    ASSERT_TRUE(spLoggerPrintDebug("MSGD","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
    ASSERT_TRUE(spLoggerPrintMsg("MSG")==SP_LOGGER_SUCCESS);
    spLoggerDestroy();
    ASSERT_TRUE(identicalFiles(testFile,expectedFile));
    return true;
}

//checks functions dont work (return logger undefined) for a closed logger
//checks multiple opening/closing logs
static bool basicLoggercloseTest() {
    const char* testFile = "basicLoggercloseTest.log";
    ASSERT_TRUE(spLoggerCreate(testFile,SP_LOGGER_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
    spLoggerDestroy();
    ASSERT_TRUE(spLoggerPrintError("MSGA","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_UNDIFINED);
    ASSERT_TRUE(spLoggerPrintWarning("MSGB","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_UNDIFINED);
    ASSERT_TRUE(spLoggerPrintInfo("MSGC") == SP_LOGGER_UNDIFINED);
    ASSERT_TRUE(spLoggerPrintDebug("MSGD","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_UNDIFINED);
    ASSERT_TRUE(spLoggerPrintMsg("MSG")==SP_LOGGER_UNDIFINED);
    ASSERT_TRUE(spLoggerCreate(testFile,SP_LOGGER_WARNING_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
    spLoggerDestroy();
    ASSERT_TRUE(spLoggerPrintError("MSGA","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_UNDIFINED);
    ASSERT_TRUE(spLoggerPrintWarning("MSGB","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_UNDIFINED);
    ASSERT_TRUE(spLoggerPrintInfo("MSGC") == SP_LOGGER_UNDIFINED);
    ASSERT_TRUE(spLoggerPrintDebug("MSGD","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_UNDIFINED);
    ASSERT_TRUE(spLoggerPrintMsg("MSG")==SP_LOGGER_UNDIFINED);


    ASSERT_TRUE(spLoggerCreate(testFile,SP_LOGGER_INFO_WARNING_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
    spLoggerDestroy();
    ASSERT_TRUE(spLoggerPrintError("MSGA","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_UNDIFINED);
    ASSERT_TRUE(spLoggerPrintWarning("MSGB","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_UNDIFINED);
    ASSERT_TRUE(spLoggerPrintInfo("MSGC") == SP_LOGGER_UNDIFINED);
    ASSERT_TRUE(spLoggerPrintDebug("MSGD","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_UNDIFINED);
    ASSERT_TRUE(spLoggerPrintMsg("MSG")==SP_LOGGER_UNDIFINED);


    ASSERT_TRUE(spLoggerCreate(testFile,SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
    spLoggerDestroy();
    ASSERT_TRUE(spLoggerPrintError("MSGA","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_UNDIFINED);
    ASSERT_TRUE(spLoggerPrintWarning("MSGB","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_UNDIFINED);
    ASSERT_TRUE(spLoggerPrintInfo("MSGC") == SP_LOGGER_UNDIFINED);
    ASSERT_TRUE(spLoggerPrintDebug("MSGD","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_UNDIFINED);
    ASSERT_TRUE(spLoggerPrintMsg("MSG")==SP_LOGGER_UNDIFINED);
    return true;
}
//checks invalid parameters are not permitted while checking the validity of all print functions with valid
//arguments in between invalid attempts
static bool invalidTest() {
    const char* expectedFile = "invalidTestExp.log";
    const char* testFile = "invalidTest.log";
    ASSERT_TRUE(spLoggerCreate(testFile,SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
    ASSERT_TRUE(spLoggerPrintError("MSG","sp_logger_unit_test.c",__func__,-1) == SP_LOGGER_INVAlID_ARGUMENT);
    ASSERT_TRUE(spLoggerPrintError("MSG","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);

    ASSERT_TRUE(spLoggerPrintError("MSG","sp_logger_unit_test.c",NULL,__LINE__) == SP_LOGGER_INVAlID_ARGUMENT);
    ASSERT_TRUE(spLoggerPrintError("MSG","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);

    ASSERT_TRUE(spLoggerPrintError("MSG",NULL,__func__,__LINE__) == SP_LOGGER_INVAlID_ARGUMENT);
    ASSERT_TRUE(spLoggerPrintError("MSG","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);

    ASSERT_TRUE(spLoggerPrintError(NULL,"sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_INVAlID_ARGUMENT);
    ASSERT_TRUE(spLoggerPrintError("MSG","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);



    ASSERT_TRUE(spLoggerPrintWarning("MSG","sp_logger_unit_test.c",__func__,-1) == SP_LOGGER_INVAlID_ARGUMENT);
    ASSERT_TRUE(spLoggerPrintWarning("MSG","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);

    ASSERT_TRUE(spLoggerPrintWarning("MSG","sp_logger_unit_test.c",NULL,__LINE__) == SP_LOGGER_INVAlID_ARGUMENT);
    ASSERT_TRUE(spLoggerPrintWarning("MSG","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);

    ASSERT_TRUE(spLoggerPrintWarning("MSG",NULL,__func__,__LINE__) == SP_LOGGER_INVAlID_ARGUMENT);
    ASSERT_TRUE(spLoggerPrintWarning("MSG","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);

    ASSERT_TRUE(spLoggerPrintWarning(NULL,"sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_INVAlID_ARGUMENT);
    ASSERT_TRUE(spLoggerPrintWarning("MSG","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);




    ASSERT_TRUE(spLoggerPrintInfo("MSGA") == SP_LOGGER_SUCCESS);


    ASSERT_TRUE(spLoggerPrintInfo("MSGB") == SP_LOGGER_SUCCESS);


    ASSERT_TRUE(spLoggerPrintInfo("MSGC") == SP_LOGGER_SUCCESS);


    ASSERT_TRUE(spLoggerPrintInfo("MSGD") == SP_LOGGER_SUCCESS);



    ASSERT_TRUE(spLoggerPrintDebug("MSG","sp_logger_unit_test.c",__func__,-1) == SP_LOGGER_INVAlID_ARGUMENT);
    ASSERT_TRUE(spLoggerPrintDebug("MSG","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);

    ASSERT_TRUE(spLoggerPrintDebug("MSG","sp_logger_unit_test.c",NULL,__LINE__) == SP_LOGGER_INVAlID_ARGUMENT);
    ASSERT_TRUE(spLoggerPrintDebug("MSG","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);

    ASSERT_TRUE(spLoggerPrintDebug("MSG",NULL,__func__,__LINE__) == SP_LOGGER_INVAlID_ARGUMENT);
    ASSERT_TRUE(spLoggerPrintDebug("MSG","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);

    ASSERT_TRUE(spLoggerPrintDebug(NULL,"sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_INVAlID_ARGUMENT);
    ASSERT_TRUE(spLoggerPrintDebug("MSG","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);

    spLoggerDestroy();
    ASSERT_TRUE(identicalFiles(testFile,expectedFile));
    return true;
}

int main() {
    RUN_TEST(basicLoggerTest);
    RUN_TEST(basicLoggerErrorTest);
    RUN_TEST(basicLoggerDebugTest);
    RUN_TEST(basicLoggerWarningTest);
    RUN_TEST(basicLoggerInfoTest);
    RUN_TEST(basicLoggercloseTest);
    RUN_TEST(invalidTest);
    return 0;
}


