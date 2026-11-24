#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "clinic.h"

/* Clears remaining characters on the current input line up to newline or EOF. */
static void flushInputLine(void) {
    int ch = 0;
    while ((ch = getchar()) != '\n' && ch != EOF) {
        /* discard extra characters */
    }
}

/* Safely copies src into dest up to maxLen - 1 chars and null-terminates. */
void copyStringSafe(char dest[], const char src[], int maxLen) {
    int i = 0;
    if (maxLen <= 0) {
        return;
    }
    while (i < maxLen - 1 && src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

/* Prompts until the user enters a valid integer in [minVal, maxVal], or returns INPUT_EOF. */
int readIntInRange(const char prompt[], int minVal, int maxVal) {
    char buffer[64];

    while (1) {
        int len = 0;
        int start = 0;
        int end = 0;
        int sign = 1;
        long value = 0;
        int valid = 1;
        int i = 0;

        printf("%s", prompt);
        if (fgets(buffer, (int)sizeof(buffer), stdin) == NULL) {
            return INPUT_EOF;
        }

        len = (int)strlen(buffer);
        if (len > 0 && buffer[len - 1] != '\n') {
            int nextCh = getchar();
            if (nextCh != '\n' && nextCh != EOF) {
                flushInputLine();
                printf("  [!] Input too long. Enter an integer between %d and %d.\n",
                       minVal, maxVal);
                continue;
            }
        }

        /* Trim leading and trailing whitespace */
        start = 0;
        while (buffer[start] != '\0' && isspace((unsigned char)buffer[start])) {
            start++;
        }
        end = len - 1;
        while (end >= start && isspace((unsigned char)buffer[end])) {
            end--;
        }

        if (start > end) {
            printf("  [!] Input cannot be blank. Enter a number (%d-%d).\n",
                   minVal, maxVal);
            continue;
        }

        i = start;
        if (buffer[i] == '-') {
            sign = -1;
            i++;
        } else if (buffer[i] == '+') {
            i++;
        }

        if (i > end) {
            printf("  [!] Invalid number. Enter an integer (%d-%d).\n",
                   minVal, maxVal);
            continue;
        }

        for (; i <= end; i++) {
            if (!isdigit((unsigned char)buffer[i])) {
                valid = 0;
                break;
            }
            /* Prevent signed overflow before multiplying */
            if (value > 100000000L) {
                valid = 0;
                break;
            }
            value = value * 10 + (buffer[i] - '0');
        }

        if (!valid) {
            printf("  [!] Invalid input. Digits only, between %d and %d.\n",
                   minVal, maxVal);
            continue;
        }

        value *= sign;
        if (value < minVal || value > maxVal) {
            printf("  [!] Out of range. Please enter a value from %d to %d.\n",
                   minVal, maxVal);
            continue;
        }

        return (int)value;
    }
}

/* Prompts until a non-empty string fitting within maxLen is entered. Returns 1 or 0 on EOF. */
int readNonEmptyString(const char prompt[], char dest[], int maxLen) {
    while (1) {
        int len = 0;
        int start = 0;
        int end = 0;
        int outIdx = 0;
        int i = 0;

        printf("%s", prompt);
        if (fgets(dest, maxLen, stdin) == NULL) {
            dest[0] = '\0';
            return 0;
        }

        len = (int)strlen(dest);
        if (len > 0 && dest[len - 1] != '\n') {
            int nextCh = getchar();
            if (nextCh != '\n' && nextCh != EOF) {
                flushInputLine();
                printf("  [!] Input exceeds %d characters. Please shorten it.\n",
                       maxLen - 1);
                continue;
            }
        }

        /* Trim leading and trailing whitespace */
        start = 0;
        while (dest[start] != '\0' && isspace((unsigned char)dest[start])) {
            start++;
        }
        end = len - 1;
        while (end >= start && isspace((unsigned char)dest[end])) {
            end--;
        }

        if (start > end) {
            printf("  [!] This field cannot be empty. Please try again.\n");
            continue;
        }

        for (i = start; i <= end; i++) {
            dest[outIdx] = dest[i];
            outIdx++;
        }
        dest[outIdx] = '\0';
        return 1;
    }
}

