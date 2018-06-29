//************************************ bs::framework - Copyright 2018 Next Limit *****************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "Leap/BsCLeapHandModel.h"
#include "Material/BsMaterial.h"
#include "Mesh/BsMesh.h"
#include "Scene/BsSceneObject.h"

namespace bs
{
	/** @addtogroup Leap
	 *  @{
	 */

	 /**
	  * A basic Leap hand model constructed dynamically vs. using pre-existing geometry.
	  */
	class CLeapCapsuleHand : public CLeapHandModelBase
	{
	public:
		/** Construct CLeapCapsuleHand. */
		CLeapCapsuleHand(const HSceneObject &parent);

		/** @copydoc CLeapHandModelBase::getChirality */
		eLeapHandType getChirality() const override { return mChirality; }

		/** @copydoc CLeapHandModelBase::getKind */
		LeapModelKind getKind() const override { return LeapModelKind::Graphics; }

		/** @copydoc CLeapHandModelBase::getLeapHand */
		const LeapHand* getLeapHand() const override { return mHand; }

		/** @copydoc CLeapHandModelBase::setLeapHand */
		void setLeapHand(const LeapHand* hand) override { mHand = hand; }

		/** @copydoc CLeapHandModelBase::init */
		void initHand() override;

		/** @copydoc CLeapHandModelBase::begin */
		void begin() override;

		/** @copydoc CLeapHandModelBase::updateHand */
		void updateHand() override;

	private:
		void onValidate();

		void drawSphere(Vector3 position);

		void drawSphere(Vector3 position, float radius);

		void drawCylinder(Vector3 a, Vector3 b);

		void drawCylinder(int a, int b);

		void drawCylinder(Vector3 a, int b);

		int getFingerJointIndex(int fingerIndex, int jointIndex);

		Map<int, HMesh> _meshMap;
		HMesh getCylinderMesh(float length);

	public:
		eLeapHandType mChirality;

	private:
		const int TOTAL_JOINT_COUNT = 4 * 5;
		const float CYLINDER_MESH_RESOLUTION = 0.1f; //in centimeters, meshes within this resolution will be re-used
		const int THUMB_BASE_INDEX = (int)LeapFinger::TYPE_THUMB * 4;
		const int PINKY_BASE_INDEX = (int)LeapFinger::TYPE_PINKY * 4;

		static const int _leftColorIndex = 0;
		static const int _rightColorIndex = 0;
		static const Color _leftColorList[];
		static const Color _rightColorList[];

		bool _showArm = true;

		bool mCastShadows = true;

		HMaterial mMaterial;

		HMesh _sphereMesh;

		int _cylinderResolution = 12;

		float _jointRadius = 0.008f;

		float _cylinderRadius = 0.006f;

		float _palmRadius = 0.015f;

		HMaterial mSphereMaterial;

		const LeapHand* mHand = NULL;

		Vector<Vector3> mSpherePositions;

		/************************************************************************/
		/* 						COMPONENT OVERRIDES                      		*/
		/************************************************************************/
	protected:
		friend class SceneObject;

		/************************************************************************/
		/* 								RTTI		                     		*/
		/************************************************************************/
	public:
		friend class CLeapCapsuleHandRTTI;
		static RTTITypeBase* getRTTIStatic();
		RTTITypeBase* getRTTI() const override;

	protected:
		CLeapCapsuleHand();// Serialization only
	};

	/** @} */
}