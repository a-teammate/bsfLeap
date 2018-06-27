//************************************ bs::framework - Copyright 2018 Next Limit *****************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "Leap/BsLeapSkeletalHand.h"

namespace bs {

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
