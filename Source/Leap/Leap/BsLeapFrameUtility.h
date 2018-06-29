//************************************ bs::framework - Copyright 2018 Next Limit *****************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "Leap/BsLeapFrame.h"

namespace bs
{
	/** @addtogroup Leap
	*  @{
	*/

	/** Performs various operations on Leap frames. */
	struct LeapFrameUtility
	{
	public:
		/** Applies transformation to LeapFrame. */
		static void transform(LeapFrame* frame, const Transform& t);

		/** Applies transformation to LeapFrame. */
		static void transform(LeapFrame* frame, const Matrix4& m);

	private:
		/** Applies transformation to LeapBone. */
		static void transform(LeapBone& bone, const Matrix4& m);

		/** Applies transformation to LeapFinger. */
		static void transform(LeapFinger& finger, const Matrix4& m);

		/** Applies transformation to LeapHand. */
		static void transform(LeapHand* hand, const Matrix4& m);
	};

	/** @} */
}