//************************************ bs::framework - Copyright 2018 Next Limit *****************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//

#include "Leap/BsCLeapCapsuleHand.h"
#include "Private/RTTI/BsCLeapCapsuleHandRTTI.h"

namespace bs
{
	const Color CLeapCapsuleHand::_leftColorList[] = { Color(0.0f, 0.0f, 1.0f), Color(0.2f, 0.0f, 0.4f), Color(0.0f, 0.2f, 0.2f) };
	const Color CLeapCapsuleHand::_rightColorList[] = { Color(1.0f, 0.0f, 0.0f), Color(1.0f, 1.0f, 0.0f), Color(1.0f, 0.5f, 0.0f) };

	CLeapCapsuleHand::CLeapCapsuleHand()
	{
		setName("LeapCapsuleHand");
	}

	CLeapCapsuleHand::CLeapCapsuleHand(const HSceneObject &parent)
		: CLeapHandModelBase(parent)
	{
		setName("LeapCapsuleHand");
	}

	/** @copydoc CLeapHandModelBase::init */

	void CLeapCapsuleHand::initHand()
	{
		if (mMaterial != NULL)
		{
			//mSphereMaterial = new Material(mMaterial);
		}
	}

	/** @copydoc CLeapHandModelBase::begin */

	void CLeapCapsuleHand::begin()
	{
		CLeapHandModelBase::begin();

		//if (mHand->mIsLeft)
		//{
		//	//mSphereMaterial->color = _leftColorList[_leftColorIndex];
		//	//_leftColorIndex = (_leftColorIndex + 1) % _leftColorList.Length;
		//}
		//else
		//{
		//	//mSphereMaterial->color = _rightColorList[_rightColorIndex];
		//	//_rightColorIndex = (_rightColorIndex + 1) % _rightColorList.Length;
		//}
	}

	/** @copydoc CLeapHandModelBase::updateHand */

	void CLeapCapsuleHand::updateHand()
	{
		if (mSpherePositions.size() != TOTAL_JOINT_COUNT)
			mSpherePositions.resize(TOTAL_JOINT_COUNT);

		//if (mSphereMaterial == NULL)
		//	mSphereMaterial = new Material(mMaterial);

		if (!mHand)
			return;

		String suffix = (mChirality == eLeapHandType_Left) ? "_L" : "_R";

		HSceneObject palm = SO()->findChild("palm");
		palm->setWorldPosition(mHand->mPalm.mPosition);

		for (int i = 0; i < 5; ++i)
		{
			const LeapFinger& leapFinger = mHand->mDigits[i];

			HSceneObject finger = SO()->findChild("finger" + toString(i));
			finger->setWorldPosition(leapFinger.mBones[0].mNextJoint);

			for (int j = 0; j < 3; ++j)
			{
				HSceneObject bone = finger->findChild("bone" + toString(j));
				bone->setWorldPosition(leapFinger.mBones[j + 1].mNextJoint);
			}
		}

		//Update all joint spheres in the fingers
		for (int i = 0; i < 5; ++i)
		{
			const LeapFinger& leapFinger = mHand->mDigits[i];
			for (int j = 0; j < 4; j++)
			{
				int key = getFingerJointIndex(i, j);

				Vector3 position = leapFinger.mBones[j].mNextJoint;
				mSpherePositions[key] = position;

				//drawSphere(position);
			}
		}

		//Now we just have a few more spheres for the hands
		//PalmPos, WristPos, and mockThumbJointPos, which is derived and not taken from the frame obj

		Vector3 palmPosition = mHand->mPalm.mPosition;
		//drawSphere(palmPosition, _palmRadius);

		Vector3 thumbBaseToPalm = mSpherePositions[THUMB_BASE_INDEX] - mHand->mPalm.mPosition;
		//Vector3 mockThumbJointPos = mHand->mPalm.mPosition + Vector3::reflect(thumbBaseToPalm, mHand->Basis.xBasis);
		//drawSphere(mockThumbJointPos);

		//If we want to show the arm, do the calculations and display the meshes
		if (_showArm)
		{
			const LeapBone& arm = mHand->mArm;

			//Vector3 right = arm.mBasis.xBasis * arm.mWidth * 0.7f * 0.5f;
			Vector3 wrist = arm.mNextJoint;
			Vector3 elbow = arm.mPrevJoint;

			float armLength = elbow.distance(wrist);
			Vector3 armDirection = (wrist - elbow);
			armDirection.normalize();
			wrist -= armDirection * armLength * 0.05f;

			//Vector3 armFrontRight = wrist + right;
			//Vector3 armFrontLeft = wrist - right;
			//Vector3 armBackRight = elbow + right;
			//Vector3 armBackLeft = elbow - right;

			//drawSphere(armFrontRight);
			//drawSphere(armFrontLeft);
			//drawSphere(armBackLeft);
			//drawSphere(armBackRight);

			//drawCylinder(armFrontLeft, armFrontRight);
			//drawCylinder(armBackLeft, armBackRight);
			//drawCylinder(armFrontLeft, armBackLeft);
			//drawCylinder(armFrontRight, armBackRight);
		}

		//Draw cylinders between finger joints
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				int keyA = getFingerJointIndex(i, j);
				int keyB = getFingerJointIndex(i, j + 1);

				Vector3 posA = mSpherePositions[keyA];
				Vector3 posB = mSpherePositions[keyB];

				//drawCylinder(posA, posB);
			}
		}

		//Draw cylinders between finger knuckles
		for (int i = 0; i < 4; i++)
		{
			int keyA = getFingerJointIndex(i, 0);
			int keyB = getFingerJointIndex(i + 1, 0);

			Vector3 posA = mSpherePositions[keyA];
			Vector3 posB = mSpherePositions[keyB];

			//drawCylinder(posA, posB);
		}

		//Draw the rest of the hand
		//drawCylinder(mockThumbJointPos, THUMB_BASE_INDEX);
		//drawCylinder(mockThumbJointPos, PINKY_BASE_INDEX);
	}

	void CLeapCapsuleHand::onValidate()
	{
		//_meshMap.Clear();
	}

	void CLeapCapsuleHand::drawSphere(Vector3 position)
	{
		drawSphere(position, _jointRadius);
	}

	void CLeapCapsuleHand::drawSphere(Vector3 position, float radius)
	{
		//multiply radius by 2 because the default unity sphere has a radius of 0.5 meters at scale 1.
		//Graphics.DrawMesh(_sphereMesh, Matrix4x4.TRS(position, Quaternion.identity,
		//	Vector3.one * radius * 2.0f * transform.lossyScale.x), mSphereMaterial, 0, NULL, 0, NULL, mCastShadows);
	}

	void CLeapCapsuleHand::drawCylinder(Vector3 a, Vector3 b)
	{
		float length = a.distance(b);

		//Graphics.DrawMesh(getCylinderMesh(length), Matrix4x4.TRS(a, Quaternion.LookRotation(b - a),
		//	Vector3(transform.lossyScale.x, transform.lossyScale.x, 1)), mMaterial, gameObject.layer, NULL, 0, NULL,
		//	mCastShadows);
	}

	void CLeapCapsuleHand::drawCylinder(int a, int b)
	{
		drawCylinder(mSpherePositions[a], mSpherePositions[b]);
	}

	void CLeapCapsuleHand::drawCylinder(Vector3 a, int b)
	{
		drawCylinder(a, mSpherePositions[b]);
	}

	int CLeapCapsuleHand::getFingerJointIndex(int fingerIndex, int jointIndex)
	{
		return fingerIndex * 4 + jointIndex;
	}

	HMesh CLeapCapsuleHand::getCylinderMesh(float length)
	{
		int lengthKey = Math::roundToInt(length * 100 / CYLINDER_MESH_RESOLUTION);

		HMesh mesh;
		//	if (_meshMap.TryGetValue(lengthKey, out mesh))
		//		return mesh;
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
		//	for (int i = 0; i < _cylinderResolution; i++)
		//	{
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

		//	_meshMap[lengthKey] = mesh;

		return mesh;
	}

	RTTITypeBase* CLeapCapsuleHand::getRTTIStatic()
	{
		return CLeapCapsuleHandRTTI::instance();
	}

	RTTITypeBase* CLeapCapsuleHand::getRTTI() const
	{
		return CLeapCapsuleHand::getRTTIStatic();
	}
}