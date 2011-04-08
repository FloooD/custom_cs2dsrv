/*
 * Published under GPLv3.
 * For more information take a look at the Readme
 * Copyright (c) by the authors of this file
 *
 * Author/s of this file: Jermuk
 */

#ifndef LOG_H_INCLUDED
#define LOG_H_INCLUDED

#include "main.h"

void myprintf(char *string, ...);
void myprintf2(char *string, ...);

/*
 #define printf(...) file = fopen("log.txt","a+");\
                    time(&rawtime );\
                    timeinfo = localtime(&rawtime);\
                    fprintf(file, "[%d:%d:%d] ", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);\
                    fprintf(file, __VA_ARGS__);\
                    printf("[%d:%d:%d] ", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);\
                    printf(__VA_ARGS__);\
                    fclose(file)

 #define eprintf(...) file = fopen("log.txt","a+");\
                    fprintf(file, __VA_ARGS__);\
                    fprintf(stdout, __VA_ARGS__);\
                    fclose(file)

 extern FILE *file;

 extern time_t rawtime;
 extern struct tm * timeinfo;
 */
#define printf(...) myprintf(__VA_ARGS__)
#define eprintf(...) myprintf2(__VA_ARGS__)

#endif // LOG_H_INCLUDED
