#ifndef __STRING_H__
#define __STRING_H__

#include <config.h>

class String {
    private:
        char data[MAX_STR_LEN];

    public:
        String() {
            data[0] = 0;
        }

        int strcpy(char *dst, char *src, int max_len = MAX_STR_LEN) {
            int i;

            for (i = 0; src[i]; i++)
                dst[i] = src[i];

            dst[i] = 0;
            return i;
        }

        void strcat(char *dst, char *src, int max_len = MAX_STR_LEN) {
            int i, j;

            for (i = 0; dst[i]; i++);

            for (j = 0; src[j] && (j < max_len); j++)
                dst[i+j] = src[j];

            dst[i+j] = 0;
        }

        void strcat_ch(char *dst, char src, int max_len = MAX_STR_LEN) {
            int i;

            for (i = 0; dst[i]; i++);
            dst[i] = src;
            i++;
            dst[i] = 0;
        }

        String& operator=(char *str) {
            strcpy(data, str);
            return *this;
        }

        String& operator+(char *str) {
            strcat(data, str);
            return *this;
        }

        String& operator+(unsigned char value) {
            strcat_ch(data, '0' + value / 100);
            strcat_ch(data, '0' + (value % 100) / 10);
            strcat_ch(data, '0' + value % 10);
            return *this;
        }

        String& operator+(char ch) {
            strcat_ch(data, ch);
            return *this;
        }

        void reset() {
            data[0] = 0;
        }

        char *c_str() {
            return data;
        }
};

#endif
