//************************************ bs::framework - Copyright 2018 Next Limit *****************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "Leap/BsLeapPrerequisites.h"

namespace bs
{
	/** @addtogroup General
	 *  @{
	 */

	/**
	 * A Limited capacity, circular LIFO buffer that wraps around when full.
	 * Supports indexing to get older items.
	 *
	 * Unlike many collections, objects are never removed, just overwritten when
	 * the buffer cycles back to their array location.
	 *
	 * Object types used must have default parameterless constructor.
	 */
	template<typename T, typename A = StdAlloc<T>>
	class CircularBuffer
	{
	public:
		/** The first template parameter (T) */
		typedef T value_type;
		/** The second template parameter (A) */
		typedef A allocator_type;
		/** value_type& */
		typedef value_type& reference;
		/** const value_type& */
		typedef const value_type& const_reference;
		/** allocator_traits<allocator_type>::pointer */
		typedef value_type* pointer;
		/** allocator_traits<allocator_type>::const_pointer */
		typedef const value_type* const_pointer;
		/** a signed integral type that can represent any non - negative value of difference_type */
		typedef int32_t size_type;

	public:
		/** Construct CircularBuffer. */
		CircularBuffer(size_type capacity) {
			mCapacity = capacity;
			mSize = 0;
			mCurrent = 0;

			mArray.resize(capacity);
		}

		/** Return size of allocated storage capacity. */
		size_type capacity() const { return mCapacity; }

		/** Test whether container is empty. */
		bool empty() const { return (mSize == 0); }

		/** Returns size. */
		size_type size() const { return mSize; }

		/** Adds a new element. Once full, this will overwrite the oldest item. */
		void push(const_reference val) {
			if (!empty()) {
				mCurrent++;
				if (mCurrent >= mCapacity) {
					mCurrent = 0;
				}
			}
			if (mSize < mCapacity)
				mSize++;

			mArray[mCurrent] = val;
		}

		/** Access element. */
		const_reference at(size_type n) const {
			assert(n < mSize);

			size_type effectiveIndex = mCurrent - n;
			if (effectiveIndex < 0) {
				effectiveIndex += mCapacity;
			}

			return mArray[effectiveIndex];
		}

		/** Access element. */
		reference at(size_type n) {
			assert(n < mSize);

			size_type effectiveIndex = mCurrent - n;
			if (effectiveIndex < 0) {
				effectiveIndex += mCapacity;
			}

			return mArray[effectiveIndex];
		}

		/** Access element. */
		const_reference top(size_type n) const {
			return at(0);
		}

		/** Access element. */
		reference top(size_type n) {
			return at(0);
		}

		/** Access element. */
		const_reference operator[](size_type n) const {
			return at(n);
		}

		/** Access element. */
		reference operator[](size_type n) {
			return at(n);
		}

		/** Change size. */
		void resize(size_type n) {
			if (n <= mCapacity) {
				return;
			}

			Vector<value_type, A> arrayNew(n);

			size_type j = 0;
			for (size_type i = mSize - 1; i >= 0; i--) {
				const_reference r = at(i);
				arrayNew[j++] = r;
			}

			mArray.swap(arrayNew);

			mCurrent = mSize - 1;
			mCapacity = n;
		}

	private:
		size_type mCurrent = 0;

		size_type mCapacity;
		size_type mSize;

		Vector<value_type, A> mArray;
	};

	/** @} */
}