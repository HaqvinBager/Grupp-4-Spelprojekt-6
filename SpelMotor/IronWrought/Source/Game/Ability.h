#pragma once
#include <string>

//class IAbility {
//
//public:
//	struct SAbilityData {
//		//Collider myCollider
//		//VFXInstance myVFX
//		//std::vector<ParticleInstance> myParticleSystems
//		//AudioInstance myAudio, or audio type
//		//Behaviour myBehaviour (movement pattern, etc.)
//		//AnimationState myAnimationState (which fbx to toggle to in the state machine)
//		float myDamage;
//		float myDuration;
//		float myRange;
//	};
//
//	virtual SAbilityData& GetAbilityData() = 0;
//	virtual void Init(std::string aFilePath) = 0;
//
//protected:
//	SAbilityData myData;
//};

class CAbility {

public:
	struct SAbilityData {
		//Collider myCollider
		//VFXInstance myVFX
		//std::vector<ParticleInstance> myParticleSystems
		//AudioInstance myAudio, or audio type
		//Behavior myBehavior (movement pattern, etc.)
		//AnimationState myAnimationState (which fbx to toggle to in the state machine)
		float myDamage;
		float myDuration;
		float myRange;
	};

	//SAbilityData& GetAbilityData();
	void Init(std::string aFilePath);
	void Update(float aDeltaTime);

private:
	SAbilityData myData;
};
