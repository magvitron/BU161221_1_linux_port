//
// UTILS_EXTRA.CPP
// Contains only the "legacy UTILS1" functions that are missing at link-time.
// IMPORTANT: Do NOT put GetPrivateProfileString/GetPrivateProfileInt/ftos/setBlink here,
// because they already exist in UTILS.CPP and will cause multiple-definition errors.
//

#include "common.h"
#include "borland_compat.h"
#include "uses.h"
#include "ct.h"
#include "utils.h"
#include "funproto.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <cmath>

// -----------------------------------------------------------------------------
// Count occurrences of a character in a string
// -----------------------------------------------------------------------------
int CharOccurInString(char *String, char ch)
{
    int nCharCount = 0;
    if (String == NULL || String[0] == '\0')
        return -1;

    for (int i = 0; String[i] != '\0'; i++)
        if (String[i] == ch)
            nCharCount++;

    return nCharCount;
}

// -----------------------------------------------------------------------------
// Reads string under multiple nested app tokens (szAppName[][]) until *_END,
// then reads key=value and returns value.
// NOTE: Original code returned pointer to local array -> BUG.
// We return a pointer to a static buffer (thread_local for safety).
// -----------------------------------------------------------------------------
char *NewGetPrivateProfileString(char szAppName[10][30],
                                char *szkeyname,
                                char *szDefault,
                                char * /*szStr*/,
                                char *FileName,
                                int nDefault)
{
    static thread_local char RetStr[150];

    if (!FileName || !szkeyname) {
        // fallback default
        if (szDefault) {
            strncpy(RetStr, szDefault, sizeof(RetStr)-1);
            RetStr[sizeof(RetStr)-1] = '\0';
            return RetStr;
        }
        RetStr[0] = '\0';
        return RetStr;
    }

    int nNumberofAppNames = 0;
    while (nNumberofAppNames < 10 && strcmp(szAppName[nNumberofAppNames], "") != 0)
        nNumberofAppNames++;

    FILE *pFile = fopen(FileName, "rt");
    if (!pFile) {
        if (szDefault) {
            strncpy(RetStr, szDefault, sizeof(RetStr)-1);
            RetStr[sizeof(RetStr)-1] = '\0';
            return RetStr;
        }
        RetStr[0] = '\0';
        return RetStr;
    }

    int *TokenFound = (int*)malloc(sizeof(int) * (nNumberofAppNames > 0 ? nNumberofAppNames : 1));
    if (!TokenFound) {
        if(pFile){fclose(pFile);}
        RetStr[0] = '\0';
        return RetStr;
    }
    memset(TokenFound, 0, sizeof(int) * (nNumberofAppNames > 0 ? nNumberofAppNames : 1));

    char cBuf[150];
    char tBuf[150];
    char *szTemp = NULL;

    // find each token sequentially
    for (int i = 0; i < nNumberofAppNames; i++) {
        strcpy(tBuf, szAppName[i]);
        strcat(tBuf, "_END");

        while (fgets(cBuf, sizeof(cBuf), pFile) != NULL) {
            szTemp = strstr(cBuf, szAppName[i]);
            if (szTemp != NULL) {
                TokenFound[i] = 1;
                break;
            }
            if (strstr(cBuf, tBuf))
                break;
        }
    }

    int flagTokenFound = 1;
    for (int i = 0; i < nNumberofAppNames; i++)
        flagTokenFound &= TokenFound[i];

    int KeyFound = 0;

    if (flagTokenFound) {
        // The last tBuf still contains the last token name + "_END"
        // Scan for key until we hit *_END
        while (fgets(cBuf, sizeof(cBuf), pFile) != NULL) {
            if (strstr(cBuf, szkeyname)) {
                szTemp = strstr(cBuf, szkeyname);
                KeyFound = 1;
                break;
            }
            if (strstr(cBuf, tBuf))
                break;
        }

        if (KeyFound && szTemp) {
            szTemp += strlen(szkeyname);

            // skip until '='
            while (*szTemp && *szTemp != '=')
                szTemp++;

            if (*szTemp == '=') {
                szTemp++; // after '='

                if (nDefault) {
                    while (*szTemp == ' ')
                        szTemp++;
                }

                // copy until newline/0
                int i = 0;
                while (*szTemp && *szTemp != '\n' && i < (int)sizeof(RetStr)-1) {
                    RetStr[i++] = *szTemp++;
                }
                RetStr[i] = '\0';

                free(TokenFound);
                if(pFile){fclose(pFile);}
                return RetStr;
            }
        }
    }

    // fallback default
    if (szDefault) {
        strncpy(RetStr, szDefault, sizeof(RetStr)-1);
        RetStr[sizeof(RetStr)-1] = '\0';
    } else {
        RetStr[0] = '\0';
    }

    free(TokenFound);
    if(pFile){fclose(pFile);}
    return RetStr;
}

// -----------------------------------------------------------------------------
// Returns a FILE* positioned just after '=' for a given key inside nested tokens.
// -----------------------------------------------------------------------------
FILE *ReturnFilePtr(char szAppName[10][30], char *szkeyname, char *FileName)
{
    if (!FileName || !szkeyname)
        return NULL;

    int nNumberofAppNames = 0;
    while (nNumberofAppNames < 10 && strcmp(szAppName[nNumberofAppNames], "") != 0)
        nNumberofAppNames++;

    FILE *pCurrFile = fopen(FileName, "rt");
    if (!pCurrFile)
        return NULL;

    int *TokenFound = (int*)malloc(sizeof(int) * (nNumberofAppNames > 0 ? nNumberofAppNames : 1));
    if (!TokenFound) {
        if(pCurrFile){fclose(pCurrFile);}
        return NULL;
    }
    memset(TokenFound, 0, sizeof(int) * (nNumberofAppNames > 0 ? nNumberofAppNames : 1));

    char cBuf[150];
    char tBuf[150];
    char *szTemp = NULL;

    for (int i = 0; i < nNumberofAppNames; i++) {
        strcpy(tBuf, szAppName[i]);
        strcat(tBuf, "_END");

        while (fgets(cBuf, sizeof(cBuf), pCurrFile) != NULL) {
            szTemp = strstr(cBuf, szAppName[i]);
            if (szTemp != NULL) {
                TokenFound[i] = 1;
                break;
            }
            if (strstr(cBuf, tBuf))
                break;
        }
    }

    int flagTokenFound = 1;
    for (int i = 0; i < nNumberofAppNames; i++)
        flagTokenFound &= TokenFound[i];

    if (!flagTokenFound) {
        free(TokenFound);
        if(pCurrFile){fclose(pCurrFile);}
        return NULL;
    }

    long lFilePrevPos = ftell(pCurrFile);
    int KeyFound = 0;

    while (fgets(cBuf, sizeof(cBuf), pCurrFile) != NULL) {
        if (strstr(cBuf, szkeyname)) {
            KeyFound = 1;
            break;
        }
        if (strstr(cBuf, tBuf))
            break;
        lFilePrevPos = ftell(pCurrFile);
    }

    if (!KeyFound) {
        free(TokenFound);
        if(pCurrFile){fclose(pCurrFile);}
        return NULL;
    }

    fseek(pCurrFile, lFilePrevPos, SEEK_SET);

    // move until '=' and leave cursor after it
    int ch;
    while ((ch = fgetc(pCurrFile)) != EOF) {
        if (ch == '=')
            break;
    }

    free(TokenFound);
    return pCurrFile; // caller must fclose()
}

// -----------------------------------------------------------------------------
// Sort float array ascending
// -----------------------------------------------------------------------------
void SortArray(float *Array, int Arraysize)
{
    if (!Array || Arraysize <= 1)
        return;

    for (int i = 0; i < Arraysize; i++) {
        for (int j = i + 1; j < Arraysize; j++) {
            if (Array[i] > Array[j]) {
                float temp = Array[i];     // FIX: float temp (was int)
                Array[i] = Array[j];
                Array[j] = temp;
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Insert Val into sorted array (keeping size), dropping the largest element.
// -----------------------------------------------------------------------------
void InsertNumber(float Val, float *Array, int Arraysize)
{
    if (!Array || Arraysize <= 0)
        return;

    if (Val >= Array[Arraysize - 1])
        return;

    int nInsertIndex;
    if (Val <= Array[0])
        nInsertIndex = 0;
    else
        nInsertIndex = Search((int)ceil((float)Arraysize / 2.0f),
                              Array, Val,
                              (int)ceil((float)Arraysize / 2.0f));

    for (int i = Arraysize - 1; i > nInsertIndex; i--)
        Array[i] = Array[i - 1];

    Array[nInsertIndex] = Val;
}

// -----------------------------------------------------------------------------
// Recursive search helper used by InsertNumber
// -----------------------------------------------------------------------------
int Search(int ArrayIndex, float *Array, float Val, int Arraysize)
{
    if (!Array || Arraysize <= 0)
        return 0;

    if (ArrayIndex > 0) {
        if (Val > Array[ArrayIndex - 1] && Val <= Array[ArrayIndex])
            return ArrayIndex;
    }

    int halfDown = (int)floor((float)Arraysize / 2.0f);
    if (halfDown == 0) halfDown = 1;

    if (Val > Array[ArrayIndex]) {
        return Search(ArrayIndex + halfDown, Array, Val, halfDown);
    } else {
        return Search(ArrayIndex - (int)ceil((float)Arraysize / 2.0f),
                      Array, Val, (int)ceil((float)Arraysize / 2.0f));
    }
}
