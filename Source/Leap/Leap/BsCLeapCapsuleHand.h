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
		eLeapHandType getChirality() const override
		{
			return mChirality;
		}

		/** @copydoc CLeapHandModelBase::getType */
		LeapModelType getType() const override
		{
			return LeapModelType::Graphics;
		}

		/** @copydoc CLeapHandModelBase::getLeapHand */
		LeapHand* getLeapHand() const override
		{
			return mHand;
		}

		/** @copydoc CLeapHandModelBase::setLeapHand */
		void setLeapHand(LeapHand* hand) override
		{
			mHand = hand;
		}

		/** @copydoc CLeapHandModelBase::init */
		void init() override
		{
			if (mMaterial != NULL) {
				//mSphereMaterial = new Material(mMaterial);
			}
		}

		/** @copydoc CLeapHandModelBase::begin */
		void begin() override
		{
			CLeapHandModelBase::begin();

			//if (mHand->mIsLeft) {
			//	//mSphereMaterial->color = _leftColorList[_leftColorIndex];
			//	//_leftColorIndex = (_leftColorIndex + 1) % _leftColorList.Length;
			//}
			//else {
			//	//mSphereMaterial->color = _rightColorList[_rightColorIndex];
			//	//_rightColorIndex = (_rightColorIndex + 1) % _rightColorList.Length;
			//}
		}

		/** @copydoc CLeapHandModelBase::update */
		void update() override
		{
			if (mSpherePositions.size() != TOTAL_JOINT_COUNT) {
				mSpherePositions.resize(TOTAL_JOINT_COUNT);
			}

			if (mSphereMaterial == NULL) {
				//mSphereMaterial = new Material(mMaterial);
			}

			if (!mHand)
				return;

			Vector3 translation(0.0f, 0.0f, 1.0f);
			Vector3 scale(0.01f, 0.01f, 0.01f);
			Transform t(translation, Quaternion::IDENTITY, scale);
			Matrix4 m = t.getMatrix();

			String suffix = (mChirality == eLeapHandType_Left) ? "_L" : "_R";

			HSceneObject palm = SO()->findChild("palm" + suffix);
			palm->setPosition(m.multiplyAffine(mHand->mPalm.mPosition));
			palm->setScale(Vector3(0.2f, 0.2f, 0.2f));

			for (UINT32 i = 0; i < 5; ++i)
			{
				HSceneObject finger = SO()->findChild("finger" + i + suffix);
				finger->setPosition(m.multiplyAffine(mHand->mDigits[i].mBones[3].mNextJoint));
				finger->setScale(Vector3(0.1f, 0.1f, 0.1f));
			}

			////Update all joint spheres in the fingers
			//foreach(var finger in mHand->Fingers) {
			//	for (int j = 0; j < 4; j++) {
			//		int key = getFingerJointIndex((int)finger.Type, j);
			//
			//		Vector3 position = finger.Bone((Bone.BoneType)j).NextJoint.ToVector3();
			//		mSpherePositions[key] = position;
			//
			//		drawSphere(position);
			//	}
			//}
			//
			////Now we just have a few more spheres for the hands
			////PalmPos, WristPos, and mockThumbJointPos, which is derived and not taken from the frame obj
			//
			//Vector3 palmPosition = mHand->PalmPosition.ToVector3();
			//drawSphere(palmPosition, _palmRadius);
			//
			//Vector3 thumbBaseToPalm = mSpherePositions[THUMB_BASE_INDEX] - mHand->PalmPosition.ToVector3();
			//Vector3 mockThumbJointPos = mHand->PalmPosition.ToVector3() + Vector3.Reflect(thumbBaseToPalm, mHand->Basis.xBasis.ToVector3());
			//drawSphere(mockThumbJointPos);
			//
			////If we want to show the arm, do the calculations and display the meshes
			//if (_showArm) {
			//	var arm = mHand->Arm;
			//
			//	Vector3 right = arm.Basis.xBasis.ToVector3() * arm.Width * 0.7f * 0.5f;
			//	Vector3 wrist = arm.WristPosition.ToVector3();
			//	Vector3 elbow = arm.ElbowPosition.ToVector3();
			//
			//	float armLength = Vector3.Distance(wrist, elbow);
			//	wrist -= arm.Direction.ToVector3() * armLength * 0.05f;
			//
			//	Vector3 armFrontRight = wrist + right;
			//	Vector3 armFrontLeft = wrist - right;
			//	Vector3 armBackRight = elbow + right;
			//	Vector3 armBackLeft = elbow - right;
			//
			//	drawSphere(armFrontRight);
			//	drawSphere(armFrontLeft);
			//	drawSphere(armBackLeft);
			//	drawSphere(armBackRight);
			//
			//	drawCylinder(armFrontLeft, armFrontRight);
			//	drawCylinder(armBackLeft, armBackRight);
			//	drawCylinder(armFrontLeft, armBackLeft);
			//	drawCylinder(armFrontRight, armBackRight);
			//}
			//
			////Draw cylinders between finger joints
			//for (int i = 0; i < 5; i++) {
			//	for (int j = 0; j < 3; j++) {
			//		int keyA = getFingerJointIndex(i, j);
			//		int keyB = getFingerJointIndex(i, j + 1);
			//
			//		Vector3 posA = mSpherePositions[keyA];
			//		Vector3 posB = mSpherePositions[keyB];
			//
			//		drawCylinder(posA, posB);
			//	}
			//}
			//
			////Draw cylinders between finger knuckles
			//for (int i = 0; i < 4; i++) {
			//	int keyA = getFingerJointIndex(i, 0);
			//	int keyB = getFingerJointIndex(i + 1, 0);
			//
			//	Vector3 posA = mSpherePositions[keyA];
			//	Vector3 posB = mSpherePositions[keyB];
			//
			//	drawCylinder(posA, posB);
			//}
			//
			////Draw the rest of the hand
			//drawCylinder(mockThumbJointPos, THUMB_BASE_INDEX);
			//drawCylinder(mockThumbJointPos, PINKY_BASE_INDEX);
		}

	private:
		void onValidate()
		{
			//_meshMap.Clear();
		}

		void drawSphere(Vector3 position)
		{
			drawSphere(position, _jointRadius);
		}

		void drawSphere(Vector3 position, float radius)
		{
			//multiply radius by 2 because the default unity sphere has a radius of 0.5 meters at scale 1.
			//Graphics.DrawMesh(_sphereMesh,
			//	Matrix4x4.TRS(position,
			//		Quaternion.identity,
			//		Vector3.one * radius * 2.0f * transform.lossyScale.x),
			//	mSphereMaterial, 0,
			//	NULL, 0, NULL, mCastShadows);
		}

		void drawCylinder(Vector3 a, Vector3 b)
		{
			//float length = (a - b).length();
			//
			//Graphics.DrawMesh(getCylinderMesh(length),
			//	Matrix4x4.TRS(a,
			//		Quaternion.LookRotation(b - a),
			//		new Vector3(transform.lossyScale.x, transform.lossyScale.x, 1)),
			//	mMaterial,
			//	gameObject.layer,
			//	NULL, 0, NULL, mCastShadows);
		}

		void drawCylinder(int a, int b)
		{
			drawCylinder(mSpherePositions[a], mSpherePositions[b]);
		}

		void drawCylinder(Vector3 a, int b)
		{
			drawCylinder(a, mSpherePositions[b]);
		}

		int getFingerJointIndex(int fingerIndex, int jointIndex)
		{
			return fingerIndex * 4 + jointIndex;
		}

		//Dictionary<int, Mesh> _meshMap = new Dictionary<int, Mesh>();
		//Mesh getCylinderMesh(float length)
		//{
		//	int lengthKey = Math::roundToInt(length * 100 / CYLINDER_MESH_RESOLUTION);
		//
		//	Mesh mesh;
		//	if (_meshMap.TryGetValue(lengthKey, out mesh)) {
		//		return mesh;
		//	}
		//
		//	mesh = new Mesh;
		//	mesh.name = "GeneratedCylinder";
		//
		//	Vector<Vector3> verts;
		//	Vector<Color> colors;
		//	Vector<int> tris;
		//
		//	Vector3 p0 = Vector3::ZERO;
		//	Vector3 p1 = -Vector3::UNIT_Z * length;
		//	for (int i = 0; i < _cylinderResolution; i++) {
		//		float angle = (Mathf.PI * 2.0f * i) / _cylinderResolution;
		//		float dx = _cylinderRadius * Mathf.Cos(angle);
		//		float dy = _cylinderRadius * Mathf.Sin(angle);
		//
		//		Vector3 spoke(dx, dy, 0);
		//
		//		verts.push_back(p0 + spoke);
		//		verts.push_back(p1 + spoke);
		//
		//		colors.push_back(Color::White);
		//		colors.push_back(Color::White);
		//
		//		int triStart = verts.size();
		//		int triCap = _cylinderResolution * 2;
		//
		//		tris.push_back((triStart + 0) % triCap);
		//		tris.push_back((triStart + 2) % triCap);
		//		tris.push_back((triStart + 1) % triCap);
		//
		//		tris.push_back((triStart + 2) % triCap);
		//		tris.push_back((triStart + 3) % triCap);
		//		tris.push_back((triStart + 1) % triCap);
		//	}
		//
		//	mesh.SetVertices(verts);
		//	mesh.SetIndices(tris.ToArray(), MeshTopology.Triangles, 0);
		//	mesh.RecalculateBounds();
		//	mesh.RecalculateNormals();
		//	mesh.UploadMeshData(true);
		//
		//	_meshMap[lengthKey] = mesh;
		//
		//	return mesh;
		//}

	public:
		eLeapHandType mChirality;

	private:
		const int TOTAL_JOINT_COUNT = 4 * 5;
		const float CYLINDER_MESH_RESOLUTION = 0.1f; //in centimeters, meshes within this resolution will be re-used
		//const int THUMB_BASE_INDEX = (int)Finger.FingerType.TYPE_THUMB * 4;
		//const int PINKY_BASE_INDEX = (int)Finger.FingerType.TYPE_PINKY * 4;

		static const int _leftColorIndex = 0;
		static const int _rightColorIndex = 0;
		//static const Color _leftColorList[] = { Color(0.0f, 0.0f, 1.0f), Color(0.2f, 0.0f, 0.4f), Color(0.0f, 0.2f, 0.2f) };
		//static const Color _rightColorList[] = { Color(1.0f, 0.0f, 0.0f), Color(1.0f, 1.0f, 0.0f), Color(1.0f, 0.5f, 0.0f) };


		bool _showArm = true;

		bool mCastShadows = true;

		Material* mMaterial;

		//Mesh _sphereMesh;

		int _cylinderResolution = 12;

		float _jointRadius = 0.008f;

		float _cylinderRadius = 0.006f;

		float _palmRadius = 0.015f;

		Material* mSphereMaterial;
		LeapHand* mHand = NULL;
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