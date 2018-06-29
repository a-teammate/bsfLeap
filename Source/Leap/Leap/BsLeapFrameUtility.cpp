//************************************ bs::framework - Copyright 2018 Next Limit *****************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//

#include "Leap/BsLeapFrameUtility.h"
#include "Math/BsMatrix4.h"
#include "Scene/BsTransform.h"

namespace bs
{
	void LeapFrameUtility::transform(LeapFrame* frame, const Transform& t)
	{
		Matrix4 m = t.getMatrix();
		transform(frame, m);
	}

	void LeapFrameUtility::transform(LeapFrame* frame, const Matrix4& m)
	{
		for (UINT32 i = 0; i < frame->mNumberOfHands; ++i)
			transform(frame->mHands + i, m);
	}

	void LeapFrameUtility::transform(LeapBone& bone, const Matrix4& m)
	{
		bone.mNextJoint = m.multiplyAffine(bone.mNextJoint);
		bone.mPrevJoint = m.multiplyAffine(bone.mPrevJoint);
	}

	void LeapFrameUtility::transform(LeapFinger& finger, const Matrix4& m)
	{
		for (UINT32 i = 0; i < 4; ++i)
			transform(finger.mBones[i], m);
	}

	void LeapFrameUtility::transform(LeapHand* hand, const Matrix4& m)
	{
		hand->mPalm.mPosition = m.multiplyAffine(hand->mPalm.mPosition);
		hand->mPalm.mStabilizedPosition = m.multiplyAffine(hand->mPalm.mStabilizedPosition);

		for (UINT32 i = 0; i < 5; ++i)
			transform(hand->mDigits[i], m);

		transform(hand->mArm, m);
	}
}