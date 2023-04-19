#ifndef PIDSHARK_UTILS_H
#define PIDSHARK_UTILS_H

sds ConvertWCToS(WCHAR* str, size_t oldStrSize) {
    size_t i = 0;
    char* newStr = (char*)malloc(oldStrSize);

    wcstombs_s(i, newStr, oldStrSize, str, oldStrSize - 1);

    sds sdsStr = sdsnew(newStr);

    free(newStr);

    return sdsStr;
}

bool IncludesStringSS(sds that, sds str) {
    return sdscmp(that, str) == 0;
}

bool IncludesStringWCS(WCHAR* that, size_t thatSize, sds str) {
    sds thatConverted = ConvertWCToS(that, thatSize);
    bool isIncluding = IncludesStringSS(thatConverted, str);
    sdsfree(thatConverted);

    return isIncluding;
}

#endif
