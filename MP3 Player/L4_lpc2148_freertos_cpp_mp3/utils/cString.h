/*
 * cString.h
 *
 * cString that acts like std::string, but uses much smaller foot-print
 * and provides parsing capability
 */

#ifndef CSTRING_H_
#define CSTRING_H_


/**
 * Extended C++ String Class.
 *
 * cString s;
 * s = "Hello World...";
 * s.trimEnd(".");
 * printf("%s", s()); // cString becomes: "Hello World"
 *
 * s.erase("World");
 * printf("%s", s()); // cString becomes: "Hello "
 * s += "World";
 * printf("%s", s()); // cString back to: "Hello World"
 *
 * Use String class as an integer:
 * cString myInt;
 * myInt = 123;
 * myInt++; if(myInt == 124)    // true
 * myInt--; if(myInt == "123")   // true
 *
 */
class cString
{
    public:
        cString();
        cString(const char* pString);
        ~cString();

        int getLen() const;   ///< @returns Number of characters in the string
        void clear();         ///< Clears the string

        void toLower(); ///< Make every alphabet character lowercase
        void toUpper(); ///< Make every alphabet character uppercase

        /**
         * @{ \name Insertion functions
         */
        void insertAtBeg(const char* pString);
        void insertAtBeg(cString s) { insertAtBeg(s()); }
        void insertAtEnd(const char* pString);
        void insertAtEnd(cString s) { insertAtEnd(s()); }
        void insertAt(const int index, const char* pString);
        void insertAt(const int index, cString s) { insertAt(index, s()); }
        /** @} */



        /**
         * @{ \name Append functions
         */
        void append(const char* pString);       ///< Appends constant string pointer
        void append(cString s) { append(s()); } ///< Appends another cString
        void append(int x);                     ///< Appends integer as characters
        void append(float x);                   ///< Appends float as characters
        void appendAsHex(unsigned int num);     ///< Appends as hexadecimal ie: DEADBEEF
        /** @} */



        /**
         * @{ \name Numeric string to numeric conversion functions
         */
        char* itoa(int value, char* result, int base);
        const char* toBaseNumberStr(const int base);
        const char* toBinaryStr() { return toBaseNumberStr(2);  }
        const char* toHexStr()    { return toBaseNumberStr(16); }
        /** @} */



        /**
         * @{ \name Comparison functions
         * @see See also the overloaded == and != operators
         */
        bool compareTo(const char* pString);
        bool compareTo(cString s) { return compareTo(s()); }
        bool compareToIgnoreCase(const char* pString);
        bool compareToIgnoreCase(cString s) { return compareToIgnoreCase(s()); }
        /** @} */



        /**
         * @{ \name Get Word Index Functions
         */
        int getWordIndexIgnoreCase(const char* pString);
        int getWordIndexIgnoreCase(cString s) { return getWordIndexIgnoreCase(s()); }

        int firstIndexOf(const char* pString);
        int firstIndexOf(cString s) { return firstIndexOf(s()); }
        int lastIndexOf(const char* pString);
        int lastIndexOf(cString s) { return lastIndexOf(s()); }
        /** @} */



        /**
         * @{ \name Functions to check a contained string
         */
        bool contains(const char* pString);
        bool contains(cString s) { return contains(s()); }
        bool containsIgnoreCase(const char* pString);
        bool containsIgnoreCase(cString s) { return containsIgnoreCase(s()); }
        bool containsWholeWord(const char* pString, const char* pSplitChars);
        bool containsWholeWordIgnoreCase(const char* pString, const char* pSplitChars);

        int countOf(const char* pString);
        int countOf(cString s) { return countOf(s()); }
        /** @} */



        /**
         * @{ \name Erase Functions
         */
        bool erase(const char* pString);
        bool erase(cString s) { return erase(s()); }
        void eraseFirst(int nChars);
        void eraseLast(int nChars);
        void eraseCharAt(int index);
        void eraseAllAfter(int index);
        void eraseAfter(int index, int nChars);
        void eraseAllSpecialChars();
        /** @} */



        /**
         * @{ \name Trimming Functions to remove leading or trailing character sets
         */
        void trimStart(const char* pChars);
        void trimStart(cString s) { trimStart(s()); }
        void trimEnd(const char* pChars);
        void trimEnd(cString s) { trimEnd(s()); }
        /** @} */



        /**
         * @{ \name Find & Replace Functions
         */
        bool replaceFirst(const char* pFind, const char* pWith);
        bool replaceLast(const char* pFind, const char* pWith);
        int replaceAll(const char* pFind, const char* pWith);
        /** @} */



        /**
         * @{ \name Sub-string Functions
         */
        const char* subString(int fromIndex);
        const char* subString(int fromIndex, int charCount);
        const char* subString(char fromFirstChar);
        const char* subString(const char* fromStr);
        const char* subString(char fromFirstChar, int charCount);
        const char* subString(const char* fromStr, int charCount);
        const char* subString(char fromFirstChar, char toLastChar);
        /** @} */



        /**
         * @{ \name Datatype detection Functions
         */
        bool isAllAlpha();     ///< @returns TRUE if the string is all alphabetical characters only
        bool isAlphaNumeric(); ///< @returns TRUE if the string is all alpha or numerical characters only
        bool isFloat(); ///< @returns TRUE if the string is a floating point number
        bool isUint();  ///< @returns TRUE if the string is an unsigned number
        bool isInt();   ///< @retuns TRUE if the string is a number
        /** @} */


        /**
         * @{ \name Encryption & Checksum Functions
         */
        void encrypt(const char* pPassword); ///< Encrypts (garbles) the string with given password
        void decrypt(const char* pPassword); ///< Encrypted string must be decrypted before further use

        unsigned int checksum_Get(); ///< Integer value of XOR checksum
        void checksum_Append();      ///< Appends checksum characters: Ex: 123 becomes: 123:0A
        void checksum_Remove();      ///< Removes checksum characters: Ex: 123:0A becomes 123
        bool checksum_Verify();      ///< @returns TRUE if for example: actual checksum 0A matches calculated 0A of string "123"
        /** @} */


        /**
         * @{ \name Assignment Operators
         */
        void operator=(const char* pString);    ///< Assign a string: myCStr = "123";
        void operator=(int num);                ///< Assign an int : myCStr = 123;
        void operator=(float num);              ///< Assign a float: myCStr = 1.23;
        cString& operator=(const cString& rhs); ///< Assign Operator for cString a = cString b
        cString(const cString& s);              ///< Copy Constructor
        /** @} */

        /**
         * @{ \name Increment & Decrement Operators if the string consist of integer value
         */
        void operator++();           ///< Pre-Increment if String is an integer
        void operator++(int unused); ///< Post-Increment if String is an integer
        void operator--();           ///< Pre-Decrement if String is an integer
        void operator--(int unused); ///< Post-Decrement if String is an integer
        /** @} */


        /**
         * Add/Subtract Operators:
         */
        void operator+=(const char singleChar); ///< Append char using += Operator
        void operator+=(const char* pString);   ///< Append String using += Operator
        void operator+=(int n);                 ///< Subtract Integer using += Operator
        void operator+=(float n);               ///< Subtract Float using += Operator
        void operator-=(const char* pString);   ///< Remove all instances of pString using -= Operator
        void operator-=(int n);                 ///< Subtract Integer String using
        void operator-=(float n);               ///< Subtract Float String using -= Operator
        /** @} */


        /**
         * @{ \name Equality Operators
         */
        bool operator==(const char* pString);
        bool operator==(int);
        bool operator!=(const char* pString);
        bool operator!=(int);
        // Note: Floats cannot be compared using == operator, they require delta comparison
        /** @} */

        /**
         * @{ \name Comparison Operators (Not done yet - TODO )
         */
        bool operator<(const char* pString);
        bool operator<(int);
        bool operator<(float);
        bool operator>(const char* pString);
        bool operator>(int);
        bool operator>(float);
        bool operator<=(const char* pString);
        bool operator<=(int);
        bool operator<=(float);
        bool operator>=(const char* pString);
        bool operator>=(int);
        bool operator>=(float);
        /** @} */

        const char* operator()() const;                   ///< () Operator: Ex: puts(myCStr());
        const char* c_str() const { return (*this)(); };  ///< Get c-string pointer (calls operator above)

        operator int();             ///< (int) Cast Operator: Ex: int x = (int)myCStr;
        char& operator[](int pos);  ///< Index Operator to get and set value @ Index

    private:
        void init(int initialLength=16);
        void ensureMemoryToInsertNChars(const int nChars);
        void ensureMemoryForBackupString(int nChars);
        void reAllocateMem(const int size);
        void copyFrom(const char* pString);

        int singleHexCharToInt(unsigned char theChar);
        int hexStrDigitsToInt(char* pString);
        void encrypt(const char* pPassword, const int lenToEncrypt);

        int mCapacity;
        int mLenOfEncryptedChars;
        int mBackupStrCapacity;
        char* mpStr;
        char* mpBackupStr;
        static const int mInvalidIndex = -1;
};

#endif /* CSTRING_H_ */
