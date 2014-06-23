/**
* \file vector.hpp
* \brief Portable Vector Class
*/

#ifndef P_VECTOR_H_
#define P_VECTOR_H_
#include <stdlib.h>

template <typename TYPE>
class VECTOR
{
public:
	VECTOR();
	VECTOR(int initialCapacity);
	VECTOR(const VECTOR& copy);          ///< Copy Constructor
	VECTOR& operator=(const VECTOR& copy); ///<  =Operator to copy a vector.
	~VECTOR();

	TYPE front();						    ///< @returns the first(oldest) element of the vector (index 0).
	TYPE back();						    ///< @returns the last added element of the vector.
	TYPE pop_front();					    ///< Pops & returns the first(oldest) element of the vector (index 0).  (SLOW)
	TYPE pop_back();					    ///< Pops & returns the last element from the vector. (FAST)
	void push_back(const TYPE& element);	///< Pushes the element to the end of the vector. (FAST)
	void push_front(const TYPE& element);	///< Pushes the element at the 1st location (index 0).  (SLOW)
	void erase(unsigned int pos);		    ///< Erases the element at the given position.  Shifts elements to the left from this point.

	unsigned int size() const;			///< @returns The size of the vector (actual usage)
	unsigned int capacity() const;      ///< @returns The capacity of the vector (allocated memory)
	void reserve(unsigned int size);	///< Reserves the memory for the vector up front.
	void setGrowthFactor(int factor);   ///< Changes the size the vector grows by.
	void clear();						///< Clears the entire vector
	bool empty();						///< @returns True if the vector is empty

	void reverse();						///< Reverses the order of the vector contents.
	TYPE rotateRight();					///< Rotates the vector right by 1 and @returns previous pop_front() element
	TYPE rotateLeft();					///< Rotates the vector left by 1 and @returns previous pop_front() element

	TYPE& operator[](const unsigned int i );				///< [] Operator for Left-hand-side.
	const TYPE& operator[](const unsigned int i ) const;	///< [] Operator of Right-hand-side.
	void operator+=(const TYPE& item) { push_back(item); }  ///< += Operator which is same as push_back() of an item

private:
	void init();
	void changeCapacity(unsigned int newSize);
	TYPE shiftLeftFromPosition(unsigned int pos);
	TYPE shiftRightFromPosition(unsigned int pos);

	unsigned int mGrowthRate;
	unsigned int mVectorCapacity;
	unsigned int mVectorSize;
	TYPE *mpData;
};















template <typename TYPE>
void VECTOR<TYPE>::init()
{
    mpData     = 0;
    mGrowthRate = 4;
    mVectorSize = 0;
    mVectorCapacity = 0;
}

template <typename TYPE>
VECTOR<TYPE>::VECTOR()
{
    init();
}
template <typename TYPE>
VECTOR<TYPE>::VECTOR(int initialCapacity)
{
    init();
	changeCapacity(initialCapacity);
}

template <typename TYPE>
VECTOR<TYPE>::VECTOR(const VECTOR& copy)
{
    init();
	*this = copy; // Call = Operator below to copy vector contents
}

template <typename TYPE>
VECTOR<TYPE>& VECTOR<TYPE>::operator=(const VECTOR<TYPE>& copy)
{
    if(this != &copy)
    {
        this->clear();
        this->reserve(copy.capacity());
        for(unsigned int i = 0; i < copy.size(); i++)
        {
            this->mpData[i] = copy[i];
        }
    }
    return *this;
}

template <typename TYPE>
VECTOR<TYPE>::~VECTOR()
{
	delete [] mpData;
}


template <typename TYPE>
TYPE VECTOR<TYPE>::pop_back()
{
	if(mVectorSize > 0)
	{
		mVectorSize--;
		return mpData[mVectorSize];
	}
	else
	{
		return 0;
	}
}


template <typename TYPE>
TYPE VECTOR<TYPE>::pop_front()
{
	if(mVectorSize > 0)
	{
		TYPE item = shiftLeftFromPosition(0);
		mVectorSize--;
		return item;
	}
	else
	{
		return 0;
	}
}

template <typename TYPE>
void VECTOR<TYPE>::push_back(const TYPE& element)
{
	if(mVectorSize >= mVectorCapacity)
	{
		changeCapacity( (mVectorCapacity + mGrowthRate) );
	}
	mpData[mVectorSize] = element;

	mVectorSize++;
}

template <typename TYPE>
void VECTOR<TYPE>::push_front(const TYPE& element)
{
	if(mVectorSize >= mVectorCapacity)
	{
		changeCapacity( (mVectorCapacity + mGrowthRate) );
	}

	// Right-most item that was shifted out is retained at the last position.
	mpData[mVectorSize] = shiftRightFromPosition(0);
	// 1st element was moved to 2nd by shifting right, place new item at 1st location.
	mpData[0] = element;
	mVectorSize++;
}

template <typename TYPE>
TYPE VECTOR<TYPE>::front()
{
	return (mVectorSize > 0) ? mpData[(mVectorSize-1)] : 0;
}

template <typename TYPE>
TYPE VECTOR<TYPE>::back()
{
	return (mVectorSize > 0) ? mpData[0] : 0;
}

template <typename TYPE>
unsigned int VECTOR<TYPE>::size() const
{
	return mVectorSize;
}

template <typename TYPE>
unsigned int VECTOR<TYPE>::capacity() const
{
	return mVectorCapacity;
}


template <typename TYPE>
void VECTOR<TYPE>::reserve(unsigned int size)
{
	changeCapacity(size);
}

template <typename TYPE>
void VECTOR<TYPE>::setGrowthFactor(int factor)
{
	if(factor > 1)
		mGrowthRate = factor;
}

template <typename TYPE>
void VECTOR<TYPE>::erase(unsigned int elementNumber)
{
	if(elementNumber < mVectorSize)
	{
		shiftLeftFromPosition(elementNumber);
		mVectorSize--;
	}
}

template <typename TYPE>
void VECTOR<TYPE>::clear()
{
	mVectorSize = 0;
}

template <typename TYPE>
bool VECTOR<TYPE>::empty()
{
	return (0 == mVectorSize);
}

template <typename TYPE>
void VECTOR<TYPE>::reverse()
{
	for(unsigned int i = 0; i < (mVectorSize/2); i++)
	{
		TYPE temp = mpData[i];
		mpData[i] = mpData[ (mVectorSize-1-i) ];
		mpData[ (mVectorSize-1-i) ] = temp;
	}
}

template <typename TYPE>
TYPE VECTOR<TYPE>::rotateLeft()
{
	if(mVectorSize >= 2)
	{
		// Shift right and set shifted element to index 0
		mpData[0] = shiftRightFromPosition(0);
		return (*this)[1]; //Previous pop_front() element
	}
	return (*this)[0];
}

template <typename TYPE>
TYPE VECTOR<TYPE>::rotateRight()
{
	if(mVectorSize >= 2)
	{
		// Shift left, and set the last element to shifted element.
		mpData[ (mVectorSize-1) ] = shiftLeftFromPosition(0);
		return (*this)[(mVectorSize-1)]; // Previous pop_front() element
	}
	return (*this)[0];
}

template <typename TYPE>
TYPE& VECTOR<TYPE>::operator[](const unsigned int i )
{
    return ( i < mVectorSize) ? mpData[i] : mpData[0];
}

template <typename TYPE>
const TYPE& VECTOR<TYPE>::operator[](const unsigned int i ) const
{
	return ( i < mVectorSize) ? mpData[i] : mpData[0];
}



// ******* PRIVATE FUNCTIONS:
template <typename TYPE>
void VECTOR<TYPE>::changeCapacity(unsigned int newSize)
{
	if(newSize < mVectorCapacity)
		return;

	// If realloc() doesn't work with C++ Class objects that contain dynamic memory inside:
#if 1
	// Allocate new memory and call TYPE's default constructor
	TYPE *newData = new TYPE[ (newSize) ];

	// Copy old memory to new one & delete old memory
	if(mpData)
	{
        for(unsigned int i = 0; i < mVectorSize; i++)
        {
            newData[i] = mpData[i];
        }
		delete [] mpData;
	}

	// Point data to new memory
	mpData = newData;
#else
	mpData = (TYPE*)realloc(mpData, sizeof(TYPE)*newSize);
#endif

	mVectorCapacity = newSize;
}

template <typename TYPE>
TYPE VECTOR<TYPE>::shiftLeftFromPosition(unsigned int pos)
{
	TYPE leftMostItem = mpData[pos];
	if(mVectorSize > 1 && (mVectorSize-pos) > 1)
	{
		// Shift elements left by one.
		for( unsigned int i = pos; i < (mVectorSize-1); i++)
		{
			mpData[i] = mpData[i+1];
		}
	}
	return leftMostItem;
}

template <typename TYPE>
TYPE VECTOR<TYPE>::shiftRightFromPosition(unsigned int pos)
{
	TYPE rightMostItem = mpData[mVectorSize-1];
	if(mVectorSize > 1)
	{
		// Shift elements right by one.
		for( unsigned int i = mVectorSize-1; i > pos; i--)
		{
			mpData[i] = mpData[i-1];
		}
	}
	return rightMostItem;
}

#endif
