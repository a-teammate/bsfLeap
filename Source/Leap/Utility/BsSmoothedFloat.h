//************************************ bs::framework - Copyright 2018 Next Limit *****************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "Leap/BsLeapPrerequisites.h"

namespace bs
{
	/** @addtogroup Leap
	*  @{
	*/

	/**
	* Time-step independent exponential smoothing.
	*/
	class SmoothedFloat
	{
	public:
		// Filtered value
		float mValue = 0.0f;
		// Mean delay
		float mDelay = 0.0f;
		// Reset on next update
		bool mReset = true;

		void setBlend(float blend, float deltaTime = 1.0f)
		{
			mDelay = deltaTime * blend / (1.0f - blend);
		}

		float update(float input, float deltaTime = 1.0f)
		{
			if (deltaTime > 0.0f && !mReset)
			{
				float alpha = mDelay / deltaTime;
				float blend = alpha / (1.0f + alpha);
				// NOTE: If mDelay -> 0 then blend -> 0,
				// reducing the filter to this.mValue = value.
				// NOTE: If deltaTime -> 0 blend -> 1,
				// so the change in the filtered value will be suppressed
				mValue = Math::lerp(1.0f - blend, mValue, input);
			}
			else
			{
				mValue = input;
				mReset = false;
			}
			return mValue;
		}
	};

	/** @} */
}