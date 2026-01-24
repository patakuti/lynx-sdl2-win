/* Test program to verify wcwidth behavior for CJK characters */
#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>

int main(void)
{
    /* Set locale for proper wcwidth behavior */
    setlocale(LC_ALL, "");

    printf("Locale: %s\n", setlocale(LC_ALL, NULL));
    printf("\n");

    /* Test characters */
    wchar_t test_chars[] = {
        L'A',     /* ASCII - should be 1 */
        L'W',     /* ASCII - should be 1 */
        L'\u3042', /* Hiragana あ - should be 2 */
        L'\u65e5', /* Kanji 日 - should be 2 */
        L'\u672c', /* Kanji 本 - should be 2 */
        L'\u8a9e', /* Kanji 語 - should be 2 */
        L'\uff21', /* Fullwidth A Ａ - should be 2 */
        L'\0'      /* Terminator */
    };

    const char *names[] = {
        "A (ASCII)",
        "W (ASCII)",
        "あ (Hiragana)",
        "日 (Kanji)",
        "本 (Kanji)",
        "語 (Kanji)",
        "Ａ (Fullwidth A)",
        NULL
    };

    printf("wcwidth results:\n");
    printf("================\n");

    for (int i = 0; test_chars[i] != L'\0'; i++) {
        int w = wcwidth(test_chars[i]);
        printf("  %s: U+%04X -> wcwidth = %d\n",
               names[i], (unsigned int)test_chars[i], w);
    }

    printf("\n");
    printf("Testing UTF-8 string width calculation:\n");
    printf("========================================\n");

    /* Test UTF-8 string */
    const char *utf8_str = "日本語テスト";
    printf("  String: %s\n", utf8_str);

    /* Convert to wchar_t and calculate width */
    wchar_t wbuf[64];
    size_t len = mbstowcs(wbuf, utf8_str, 63);

    if (len != (size_t)-1) {
        int total_width = 0;
        for (size_t i = 0; i < len; i++) {
            int w = wcwidth(wbuf[i]);
            printf("    wbuf[%zu] = U+%04X, wcwidth = %d\n",
                   i, (unsigned int)wbuf[i], w);
            if (w > 0) total_width += w;
        }
        printf("  Total display width: %d cells (char count: %zu)\n",
               total_width, len);
    } else {
        printf("  mbstowcs failed\n");
    }

    return 0;
}
