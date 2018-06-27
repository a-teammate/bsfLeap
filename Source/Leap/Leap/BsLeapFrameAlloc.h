//************************************ bs::framework - Copyright 2018 Next Limit *****************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "Leap/BsLeapPrerequisites.h"
#include "Leap/BsLeapTypes.h"

namespace bs
{
	/** @addtogroup Leap
	 *  @{
	 */

	 /**
	 * Class representing a memory buffer used when copying LeapFrame from LeapC.
	 */
	template <typename A = StdAlloc<uint8_t>>
	class LeapFrameAllocT
	{
	public:
		/** Returns the frame. */
		const LeapFrame* get() const;

		/** Returns the frame. */
		LeapFrame* get();

		void copyFrom(const LEAP_TRACKING_EVENT* trackingEvent);

		/** Allocates memory needed to save the LeapFrame data. */
		void resize(size_t size);

		/** Allocates memory needed to save the LeapFrame data. */
		void resizeNumberOfHands(uint32_t nHands);

		/** Returns the number of bytes needed to allocate a LeapFrame data. */
		static size_t sizeBytesNumberOfHands(uint32_t nHands);

	private:
		/** Memory buffer for saving LEAP_TRACKING_EVENT data returned from LeapC */
		Vector<uint8_t, A> mAlloc;
	};

	/** LeapFrameAlloc with default allocator. */
	typedef LeapFrameAllocT<> LeapFrameAlloc;

	/** @} */
}