#include "cString.h"
#include <string.h> // memcpy, strcmp
#include <ctype.h>  // tolower/toupper
#include <stdlib.h> // realloc()
#include <stdio.h>  // sprintf
#include <stdlib.h> // atoi() atof()

cString::cString()
{
    init();
}

cString::cString(const char* pString)
{
    init(strlen(pString));
    copyFrom(pString);
}

cString::~cString()
{
    if(0 != mpStr) {
        delete mpStr;
    }
    if(0 != mpBackupStr) {
        delete mpBackupStr;
    }
}

int cString::getLen() const
{
    return strlen(mpStr);
}

void cString::clear()
{
    memset(mpStr, 0, mCapacity);
}

void cString::toLower()
{
    const int len = strlen(mpStr);
    for(int i = 0; i < len; i++) {
        mpStr[i] = tolower(mpStr[i]);
    }
}

void cString::toUpper()
{
    const int len = strlen(mpStr);
    for(int i = 0; i < len; i++) {
        mpStr[i] = toupper(mpStr[i]);
    }
}

void cString::insertAtBeg(const char* pString)
{
    insertAt(0, pString);
}

void cString::insertAtEnd(const char* pString)
{
    ensureMemoryToInsertNChars(strlen(pString));
    strcat(mpStr, pString);
}

void cString::insertAt(const int index, const char* pString)
{
    const int newLen = strlen(pString);
    ensureMemoryToInsertNChars(newLen);

    if(index >= 0 && index <= getLen() && newLen > 0)
    {
        // "Hello", insert at 2 "123" ==> "He123llo"
        void* destToMakeRoom = mpStr + index + newLen;
        void* srcToCopyLeftOverString = mpStr + index;
        const int lenToMove = getLen() - index + 1; // +1 for NULL Char
        memmove(destToMakeRoom, srcToCopyLeftOverString, lenToMove);

        memcpy(mpStr + index, pString, newLen);
    }
}

void cString::append(const char* pString)
{
    insertAtEnd(pString);
}

void cString::append(int x)
{
    char intValString[32];
    sprintf(intValString, "%i", x);
    append(intValString);
}

void cString::append(float x)
{
    char floatValString[32];
    sprintf(floatValString, "%f", x);
    append(floatValString);
}

void cString::appendAsHex(unsigned int num)
{
    char hexVal[32];
    sprintf(hexVal, "%02X", num);
    append(hexVal);
}

char* cString::itoa(int value, char* result, int base)
{
	/**
	 * C++ version 0.4 char* style "itoa":
	 * Written by Lukás Chmela
	 * Released under GPLv3.
	 */
	// check that the base if valid
	if (base < 2 || base > 36) { *result = '\0'; return result; }

	char* ptr = result, *ptr1 = result, tmp_char;
	int tmp_value;

	do {
		tmp_value = value;
		value /= base;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
	} while ( value );

	// Apply negative sign
	if (tmp_value < 0) *ptr++ = '-';
	*ptr-- = '\0';
	while(ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr--= *ptr1;
		*ptr1++ = tmp_char;
	}
	return result;
}

const char* cString::toBaseNumberStr(const int base)
{
    const int maxBitsFor32BitBinary = 32;
    const int neededStrMem = maxBitsFor32BitBinary + 1; // NULL
    ensureMemoryForBackupString(neededStrMem);

    char* pRetStr = mpBackupStr;
    *pRetStr = '\0';

    if(isInt())
    {
        itoa((int)(*this), pRetStr, base);
    }

    return pRetStr;
}

bool cString::compareTo(const char* pString)
{
    return (0 == strcmp(pString, mpStr));
}

bool cString::compareToIgnoreCase(const char* pString)
{
    const int ourLen = getLen();
    const int theirLen = strlen(pString);

    if(ourLen != theirLen) {
        return false;
    }

    for(int i = 0; i < ourLen; i++) {
        if(tolower(mpStr[i]) != tolower(pString[i])) {
            return false;
        }
    }

    return true;
}

bool cString::contains(const char* pString)
{
    return firstIndexOf(pString) != mInvalidIndex;
}

int cString::getWordIndexIgnoreCase(const char* pString)
{
    int findIndex = mInvalidIndex;
    char* firstChar;

    if(0 != *pString && 0 != *mpStr)
    {
        for(char* pSearchLoc = mpStr; 0 != *pSearchLoc; pSearchLoc = firstChar + 1)
        {
            // First, find first char match ignoring the case.
            if(0 == (firstChar = strchr(pSearchLoc, tolower(*pString)))) {
                if(0 == (firstChar = strchr(pSearchLoc, toupper(*pString)))) {
                    findIndex = mInvalidIndex;
                    break;
                }
            }

            // First char matched, now match remaining string
            char* i2 = (char*)pString + 1;
            findIndex = firstChar - pSearchLoc;

            for(char* i1 = firstChar + 1; *i1 && *i2; i1++, i2++)
            {
                if(tolower(*i1) != tolower(*i2))
                {
                    findIndex = mInvalidIndex;
                    break;
                }
            }

            // If iterator2 reached null char, we matched entire string!
            if(0 == *i2) {
                break;
            }
        };
    }

    return findIndex;
}

bool cString::containsIgnoreCase(const char* pString)
{
    return getWordIndexIgnoreCase(pString) != mInvalidIndex;
}

int cString::firstIndexOf(const char* pString)
{
    char* pFindPtr = strstr(mpStr, pString);
    return (0 == pFindPtr) ? mInvalidIndex : (pFindPtr - mpStr);
}

int cString::lastIndexOf(const char* pString)
{
    const int lenOfStrToFind = strlen(pString);
    char* pFindPtr = strstr(mpStr, pString);
    char* pLastPtr = pFindPtr;

    while(0 != pFindPtr)
    {
        pLastPtr = pFindPtr;
        pFindPtr = strstr(pFindPtr + lenOfStrToFind, pString);
    }

    return (0 == pLastPtr) ? mInvalidIndex : (pLastPtr - mpStr);
}

int cString::countOf(const char* pString)
{
    const int lenOfStrToFind = strlen(pString);
    char* pFindPtr = strstr(mpStr, pString);
    int count = 0;

    while(0 != pFindPtr)
    {
        count++;
        pFindPtr = strstr(pFindPtr + lenOfStrToFind, pString);
    }

    return count;
}

bool cString::erase(const char* pString)
{
    const int findIndex = firstIndexOf(pString);
    if(mInvalidIndex != findIndex)
    {
        eraseAfter(findIndex, strlen(pString));
        return true;
    }
    return false;
}

void cString::eraseFirst(int nChars)
{
    const int len = getLen();
    if(nChars > 0 && nChars <= len)
    {
        memmove(mpStr, mpStr + nChars, len - nChars + 1);
    }
}

void cString::eraseLast(int nChars)
{
    const int len = getLen();
    if(nChars <= len)
    {
        mpStr[len-nChars] = '\0';
    }
}

void cString::eraseCharAt(int index)
{
    eraseAfter(index, 1);
}

void cString::eraseAllAfter(int index)
{
    if(index >= 0 && index < getLen())
    {
        mpStr[index] = '\0';
    }
}

void cString::eraseAfter(int index, int nChars)
{
    const int ourLen = getLen();
    if(index >= 0 && index < ourLen && nChars > 0)
    {
        // "012345", index = 2, nChars = 6
        if(nChars > ourLen - index) {
            nChars = ourLen - index;
        }
        memmove(mpStr+index, mpStr+index+nChars, ourLen - index - nChars + 1);
    }
}

void cString::eraseAllSpecialChars()
{
    for(int i = 0; i < getLen(); i++)
    {
        const char thisChar = mpStr[i];
        if(!(thisChar >= 'A' && thisChar <= 'z') && ! (thisChar >= '0' && thisChar <= '9'))
        {
            // TODO: Optimize.
            // TODO: Bug because getLen() changes, test this case.
            eraseCharAt(i);
        }
    }
}

void cString::trimStart(const char* pChars)
{
    int numBegCharsToRemove = 0;
    const int ourLen = getLen();

    for(int i = 0; i < ourLen; i++)
    {
        const int len = strlen(pChars);
        bool trimDone = false;

        for(int j = 0 ; j < len; j++)
        {
            if(pChars[j] == mpStr[i]) {
                trimDone = true;
                numBegCharsToRemove++;
                break;
            }
        }

        if(!trimDone) {
            break;
        }
    }

    if(numBegCharsToRemove > 0) {
        eraseFirst(numBegCharsToRemove);
    }
}

void cString::trimEnd(const char* pChars)
{
    const int trimLen = strlen(pChars);
    for(int i = getLen() - 1; i >= 0; i--)
    {
        bool trimDone = false;
        for(int j = 0 ; j < trimLen; j++)
        {
            if(pChars[j] == mpStr[i])
            {
                trimDone = true;
                mpStr[i] = '\0';
                break;
            }
        }

        // If no trim took place this iteration, break here
        if(!trimDone) {
            break;
        }
    }
}

bool cString::replaceFirst(const char* pFind, const char* pWith)
{
    const int findIndex = firstIndexOf(pFind);
    if(findIndex != mInvalidIndex)
    {
        eraseAfter(findIndex, strlen(pFind));
        insertAt(findIndex, pWith);
        return true;
    }
    return false;
}

bool cString::replaceLast(const char* pFind, const char* pWith)
{
    const int findIndex = lastIndexOf(pFind);
    if(findIndex != mInvalidIndex)
    {
        eraseAfter(findIndex, strlen(pFind));
        insertAt(findIndex, pWith);
        return true;
    }
    return false;
}

int cString::replaceAll(const char* pFind, const char* pWith)
{
    int count = 0;
    int findIndex = mInvalidIndex;

    while(mInvalidIndex != (findIndex = firstIndexOf(pFind)))
    {
        eraseAfter(findIndex, strlen(pFind));
        insertAt(findIndex, pWith);
        ++count;
    }

    return count;
}

const char* cString::subString(int fromIndex)
{
    // Point to empty string by default
    char* pSubStr = mpBackupStr;
    *pSubStr = '\0';

    if(fromIndex >= 0 && fromIndex < getLen()) {
        pSubStr = mpStr + fromIndex;
    }
    return pSubStr;
}

const char* cString::subString(int fromIndex, int charCount)
{
    // Point to empty string by default
    char* pSubStr = mpBackupStr;
    *pSubStr = '\0';

    if(charCount > 0 && fromIndex >= 0 && fromIndex < getLen()) {
        ensureMemoryForBackupString(charCount);
        pSubStr = strncpy(mpBackupStr, mpStr + fromIndex, charCount);

        // Mark last char as NULL
        *(pSubStr + charCount) = '\0';
    }
    return pSubStr;
}

const char* cString::subString(char fromFirstChar)
{
    const char str[] = {fromFirstChar, 0x00};
    return subString(str);
}

const char* cString::subString(const char* fromStr)
{
    return subString(firstIndexOf(fromStr));
}

const char* cString::subString(char fromFirstChar, int charCount)
{
    const char str[] = {fromFirstChar, 0x00};
    return subString(str, charCount);
}

const char* cString::subString(const char* fromStr, int charCount)
{
    return subString(firstIndexOf(fromStr), charCount);
}

const char* cString::subString(char fromFirstChar, char toLastChar)
{
    const char fi[] = {fromFirstChar, 0x00};
    const char li[] = {toLastChar, 0x00};
    const int firstIndex = firstIndexOf(fi);
    const int lastIndex  =  lastIndexOf(li);

    // Point to empty string by default
    char* pSubStr = mpBackupStr;
    *pSubStr = '\0';

    if(firstIndex >=0 && lastIndex >= 0) {
        pSubStr = (char*)subString(firstIndex, lastIndex - firstIndex + 1);
    }

    return pSubStr;
}

bool cString::isAllAlpha()
{
    const int ourLen = getLen();
    bool isAlpha = true;

    for(int i = 0; i < ourLen; i++)
    {
        const char thisChar = mpStr[i];
        if(! (thisChar >= 'A' && thisChar <= 'z'))
        {
            isAlpha = false;
            break;
        }
    }

    return isAlpha;
}
bool cString::isAlphaNumeric()
{
    const int ourLen = getLen();
    bool isAlphaNumero = true;

    for(int i = 0; i < ourLen; i++)
    {
        const char thisChar = mpStr[i];
        if(!(thisChar >= 'A' && thisChar <= 'z') && ! (thisChar >= '0' && thisChar <= '9'))
        {
            isAlphaNumero = false;
            break;
        }
    }

    return isAlphaNumero;
}
bool cString::isFloat()
{
    const int ourLen = getLen();
    bool strIsAFloat = countOf(".") <= 1;

    if(strIsAFloat)
    {
        for(int i = mpStr[0] == '-' ? 1 : 0; i < ourLen; i++)
        {
            const char thisChar = mpStr[i];
            if(thisChar != '.' && ! (thisChar >= '0' && thisChar <= '9'))
            {
                strIsAFloat = false;
                break;
            }
        }
    }

    return strIsAFloat;
}
bool cString::isUint()
{
    const int ourLen = getLen();
    bool isAllDigits = true;

    for(int i = 0; i < ourLen; i++)
    {
        const char thisChar = mpStr[i];
        if(! (thisChar >= '0' && thisChar <= '9'))
        {
            isAllDigits = false;
            break;
        }
    }

    return isAllDigits;
}
bool cString::isInt()
{
    const int ourLen = getLen();
    bool strIsAnInt = true;

    for(int i = mpStr[0] == '-' ? 1 : 0; i < ourLen; i++)
    {
        const char thisChar = mpStr[i];
        if(! (thisChar >= '0' && thisChar <= '9'))
        {
            strIsAnInt = false;
            break;
        }
    }

    return strIsAnInt;
}

void cString::encrypt(const char* pPassword)
{
    mLenOfEncryptedChars = getLen();
    encrypt(pPassword, mLenOfEncryptedChars);

}

void cString::decrypt(const char* pPassword)
{
    // Decrypt is same thing as encrypt to get original string back
    encrypt(pPassword, mLenOfEncryptedChars);
}

unsigned int cString::checksum_Get()
{
    int chkSum = 0;
    const int ourLen = getLen();
    for(int i = 0; i < ourLen; i++) {
        chkSum xor_eq mpStr[i];
    }
    return chkSum;
}

void cString::checksum_Append()
{
    const unsigned int chksum = checksum_Get();
    append(":");
    appendAsHex(chksum);
}

void cString::checksum_Remove()
{
    eraseAllAfter(lastIndexOf(":"));
}

bool cString::checksum_Verify()
{
    bool checksumIsValid = false;
    const int checkSumIndex = lastIndexOf(":");

    if(mInvalidIndex != checkSumIndex)
    {
        mpStr[checkSumIndex] = '\0';
        const unsigned int actualChecksum = hexStrDigitsToInt(mpStr+checkSumIndex+1);
        const unsigned int expectedChecksum = checksum_Get();
        checksumIsValid = (actualChecksum == expectedChecksum);
        mpStr[checkSumIndex] = ':';
    }

    return checksumIsValid;
}



/*****************************************************************/
/******************* Operator Overloading ************************/
/*****************************************************************/
void cString::operator=(const char* pString)
{
    copyFrom(pString);
}
void cString::operator=(int num)
{
    char buff[32];
    itoa(num, buff, 10);
    copyFrom(buff);
}
void cString::operator=(float num)
{
    char buff[32];
    sprintf(buff, "%f", num);
    copyFrom(buff);
}

void cString::operator++()
{
    (*this) += 1; // Call += Operator Below
}
void cString::operator++(int unused)
{
    (*this) += 1; // Call += Operator Below
}

void cString::operator--()
{
    (*this) -= 1; // Call -= Operator Below
}
void cString::operator--(int unused)
{
    (*this) -= 1; // Call -= Operator Below
}



void cString::operator+=(const char* pString)
{
    append(pString);
}
void cString::operator+=(const char singleChar)
{
    char s[] = {singleChar, '\0'};
    append(s);
}
void cString::operator+=(int n)
{
    if(isInt())
    {
        (*this) = atoi(mpStr) + n; // Call Assignment Operator for int
    }
}
void cString::operator+=(float n)
{
    if(isFloat())
    {
        (*this) = (float)atof(mpStr) + n; // Call Assignment Operator for float
    }
}
void cString::operator-=(const char* pString)
{
    erase(pString);
}
void cString::operator-=(int n)
{
    if(isInt())
    {
        (*this) = atoi(mpStr) - n; // Call Assignment Operator for int
    }
}
void cString::operator-=(float n)
{
    if(isFloat())
    {
        (*this) = (float)atof(mpStr) - n; // Call Assignment Operator for float
    }
}


bool cString::operator==(const char* pString)
{
    return compareTo(pString);
}
bool cString::operator==(int n)
{
    return atoi(mpStr) == n;
}
bool cString::operator!=(const char* pString)
{
    return !((*this) == pString); // Call ==Operator for char*
}
bool cString::operator!=(int n)
{
    return !((*this) == n); // Call ==Operator for int
}
/*
// Note: Floats cannot be compared using == operator, they require delta comparison
bool cString::operator==(float n)
{
    return atof(mpStr) == n;
}
bool cString::operator!=(float n)
{
    return !((*this) == n); // Call ==Operator for float
}
*/

const char* cString::operator()() const
{
    return mpStr;
}

cString::operator int()
{
    return atoi(mpStr);
}

char& cString::operator[](int pos)
{
    return pos < getLen() ? mpStr[pos] : mpStr[0];
}

cString& cString::operator=(const cString& copy)
{
    // For the assignment operator, we need to copy string data memory into our memory
    // The rest of the member variables do not matter and they will auto-adjust
    if(this != &copy)
    {
        // Same as: this->copyFrom(copy.get());
        *this = copy();
    }
    return *this;
}

cString::cString(const cString& copy)
{
    // Allocate enough memory first
    this->init(copy.getLen());

    // Simply use assignment operator code above
    *this = copy;
}

// Private
void cString::init(int initialLength)
{
    mCapacity = 0;
    mLenOfEncryptedChars = 0;
    mBackupStrCapacity = 0;
    mpStr = 0;
    mpBackupStr = 0;

    reAllocateMem(initialLength);
}

void cString::ensureMemoryToInsertNChars(const int nChars)
{
    const int newLen = nChars;
    const int existingLen = getLen();
    const int requiredMem = newLen + existingLen;

    if(mCapacity < requiredMem)
    {
        reAllocateMem(requiredMem);
    }
}

void cString::ensureMemoryForBackupString(int nChars)
{
    if(nChars > mBackupStrCapacity) {
        mBackupStrCapacity = nChars;
        mpBackupStr = (char*)realloc(mpBackupStr, nChars);
    }
}

void cString::reAllocateMem(const int size)
{
    // +1 for NULL character
    mCapacity = size;
    if(0 == mpStr){
        mpStr = (char*)malloc(mCapacity + 1);
        memset(mpStr, 0, mCapacity);
    }
    else {
        mpStr = (char*)realloc(mpStr, mCapacity + 1);
    }
}

void cString::copyFrom(const char* pString)
{
    const int strLen = strlen(pString);

    if(strLen > mCapacity) {
        reAllocateMem(strLen);
    }

    // Copy +1 for Null Char too
    memcpy(mpStr, pString, strLen+1);
}

int cString::singleHexCharToInt(unsigned char theChar)
{
    if(theChar >= 'a' && theChar <= 'f') {
        theChar -= 'a';
        theChar += 10;
    }
    else if(theChar >= 'A' && theChar <= 'F') {
        theChar -= 'A';
        theChar += 10;
    }
    else {
        theChar -= '0';
    }
    return theChar;
}
int cString::hexStrDigitsToInt(char* pString)
{
    const char leftChar = pString[0];
    const char rightChar = pString[1];
    return (singleHexCharToInt(leftChar) << 4) + singleHexCharToInt(rightChar);
}

void cString::encrypt(const char* pPassword, const int lenToEncrypt)
{
    const int passLen = strlen(pPassword);
    for(int i = 0; i < lenToEncrypt; i++) {
        for(int j = 0; j < passLen; j++) {
            mpStr[i] xor_eq pPassword[j];
        }
    }
}
