//************************************ bs::framework - Copyright 2018 Next Limit *****************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "Leap/BsLeapHand.h"

namespace bs {

/**
 * A hand object consisting of discrete, component parts.
 *
 * The hand can have game objects for the palm, wrist and forearm, as well as
 * fingers.
 */
class LeapSkeletalHand : CLeapHandModel {
public
  override ModelType HandModelType {
    get { return ModelType.Graphics; }
  }
protected
  const float PALM_CENTER_OFFSET = 0.015f;

  void Start() {
    // Ignore collisions with self.
    Leap.Unity.Utils.IgnoreCollisions(gameObject, gameObject);

    for (int i = 0; i < fingers.Length; ++i) {
      if (fingers[i] != null) {
        fingers[i].fingerType = (Finger.FingerType)i;
      }
    }
  }

  /** Updates the hand and its component parts by setting their positions and
   * rotations. */
public
  override void UpdateHand() { SetPositions(); }

protected
  Vector3 GetPalmCenter() {
    Vector3 offset = PALM_CENTER_OFFSET *
                     Vector3.Scale(GetPalmDirection(), transform.lossyScale);
    return GetPalmPosition() - offset;
  }

protected
  void SetPositions() {
    for (int f = 0; f < fingers.Length; ++f) {
      if (fingers[f] != null) {
        fingers[f].UpdateFinger();
      }
    }

    if (palm != null) {
      palm.position = GetPalmCenter();
      palm.rotation = GetPalmRotation();
    }

    if (wristJoint != null) {
      wristJoint.position = GetWristPosition();
      wristJoint.rotation = GetPalmRotation();
    }

    if (forearm != null) {
      forearm.position = GetArmCenter();
      forearm.rotation = GetArmRotation();
    }
  }
}

/** A physics model for a hand made out of various Rigidbody collider. */
class LeapRigidHand : LeapSkeletalHand {
public:
  LeapModelType getHandModelType { return LeapModelType.Physics; }

  float filtering = 0.5f;

  bool SupportsEditorPersistence() { return true; }

  void InitHand() { base.InitHand(); }

  void UpdateHand() {
    for (int f = 0; f < fingers.Length; ++f) {
      if (fingers[f] != null) {
        fingers[f].UpdateFinger();
      }
    }

    if (palm != null) {
      Rigidbody palmBody = palm.GetComponent<Rigidbody>();
      if (palmBody) {
        palmBody.MovePosition(GetPalmCenter());
        palmBody.MoveRotation(GetPalmRotation());
      } else {
        palm.position = GetPalmCenter();
        palm.rotation = GetPalmRotation();
      }
    }

    if (forearm != null) {
      // Set arm dimensions.
      CapsuleCollider capsule = forearm.GetComponent<CapsuleCollider>();
      if (capsule != null) {
        // Initialization
        capsule.direction = 2;
        forearm.localScale = new Vector3(1f / transform.lossyScale.x,
                                         1f / transform.lossyScale.y,
                                         1f / transform.lossyScale.z);

        // Update
        capsule.radius = GetArmWidth() / 2f;
        capsule.height = GetArmLength() + GetArmWidth();
      }

      Rigidbody forearmBody = forearm.GetComponent<Rigidbody>();
      if (forearmBody) {
        forearmBody.MovePosition(GetArmCenter());
        forearmBody.MoveRotation(GetArmRotation());
      } else {
        forearm.position = GetArmCenter();
        forearm.rotation = GetArmRotation();
      }
    }
  }
}

}
