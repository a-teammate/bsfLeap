//************************************ bs::framework - Copyright 2018 Next Limit *****************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "Leap/BsLeapPrerequisites.h"
#include "Leap/BsLeapFrame.h"

namespace bs
{
	/** @addtogroup Leap
	 *  @{
	 */

	 /**
	 * Class representing a memory buffer used when copying LeapFrame from LeapC.
	 */
	template <typename A = StdAlloc<UINT8>>
	class LeapFrameAllocT
	{
	public:
		LeapFrameAllocT() = default;
		LeapFrameAllocT(const LeapFrameAllocT&) = delete;

		/** Returns the LeapFrame. */
		const LeapFrame* get() const;

		/** Returns the LeapFrame. */
		LeapFrame* get();

		/** Updates the LeapFrame data with the provided LEAP_TRACKING_EVENT. */
		void copyFrom(const LEAP_TRACKING_EVENT* trackingEvent);

		/** Allocates memory needed to save the LeapFrame data. */
		void resize(size_t size);

		/** Allocates memory needed to save the LeapFrame data. */
		void resizeNumberOfHands(UINT32 nHands);

		/** Copy assignment. */
		LeapFrameAllocT& operator=(const LeapFrameAllocT& other);

		/** Copy assignment. */
		void _copy(const LeapFrameAllocT& other);

		/** Returns the number of bytes needed to allocate a LeapFrame data. */
		size_t _sizeBytesNumberOfHands(UINT32 nHands) const;

	private:
		/** Memory buffer for saving LeapFrame data. */
		Vector<UINT8, A> mAlloc;
	};

	/** LeapFrameAlloc with default allocator. */
	typedef LeapFrameAllocT<> LeapFrameAlloc;

	/** @} */
}