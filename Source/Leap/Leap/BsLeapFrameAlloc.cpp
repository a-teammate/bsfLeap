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
		UINT8* data = mAlloc.data();
		
		std::memcpy(data, trackingEvent, sizeof(LeapFrame));

		data += sizeof(LeapFrame);
		get()->mHands = reinterpret_cast<LeapHand*>(data);

		for (UINT32 i = 0; i < trackingEvent->nHands; ++i)
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
	void LeapFrameAllocT<A>::resizeNumberOfHands(UINT32 nHands)
	{
		resize(_sizeBytesNumberOfHands(nHands));
	}

	template <typename A>
	LeapFrameAllocT<A>& LeapFrameAllocT<A>::operator=(const LeapFrameAllocT& other)
	{
		if (this != &other)
			this->_copy(other);

		return *this;
	}

	template <typename A>
	void LeapFrameAllocT<A>::_copy(const LeapFrameAllocT& other)
	{
		copyFrom(reinterpret_cast<const LEAP_TRACKING_EVENT*>(other.get()));
	}

	template <typename A>
	size_t LeapFrameAllocT<A>::_sizeBytesNumberOfHands(UINT32 nHands) const
	{
		return sizeof(LeapFrame) + nHands * sizeof(LeapHand);
	}

	template class LeapFrameAllocT<StdAlloc<UINT8>>;
}