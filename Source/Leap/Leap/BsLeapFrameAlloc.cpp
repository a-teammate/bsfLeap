//************************************ bs::framework - Copyright 2018 Next Limit *****************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//

#include "Leap/BsLeapFrameAlloc.h"

namespace bs
{
	template <typename A>
	const LeapFrame* LeapFrameAllocT<A>::get() const
	{
		return reinterpret_cast<const LeapFrame*>(mAlloc.data());
	}

	template <typename A>
	LeapFrame* LeapFrameAllocT<A>::get()
	{
		return reinterpret_cast<LeapFrame*>(mAlloc.data());
	}

	template <typename A>
	void LeapFrameAllocT<A>::copyFrom(const LEAP_TRACKING_EVENT* trackingEvent)
	{
		resizeNumberOfHands(trackingEvent->nHands);
		uint8_t* data = mAlloc.data();
		
		std::memcpy(data, trackingEvent, sizeof(LeapFrame));

		data += sizeof(LeapFrame);
		get()->mHands = reinterpret_cast<LeapHand*>(data);

		for (uint32_t i = 0; i < trackingEvent->nHands; ++i)
		{
			std::memcpy(data, trackingEvent->pHands + i, sizeof(LeapHand));
			data += sizeof(LeapHand);
		}
	}

	template <typename A>
	void LeapFrameAllocT<A>::resize(size_t size)
	{
		mAlloc.clear();
		mAlloc.resize(size);
	}

	template <typename A>
	void LeapFrameAllocT<A>::resizeNumberOfHands(uint32_t nHands)
	{
		resize(sizeBytesNumberOfHands(nHands));
	}

	template <typename A>
	size_t LeapFrameAllocT<A>::sizeBytesNumberOfHands(uint32_t nHands)
	{
		return sizeof(LeapFrame) + nHands * sizeof(LeapHand);
	}

	template class LeapFrameAllocT<StdAlloc<uint8_t>>;
}