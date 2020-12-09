using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public struct BossData 
{
    public int myInstanceID;
    public Vector3 myPosition;
    public Vector3 myRotation;
    public Vector3 myScale;
    public Vector3 myStartPhases;
    public Vector3 myEndPhases;

    public BossData(BossSetup aData)
    {
        myInstanceID = aData.gameObject.GetInstanceID();
        myPosition = aData.transform.position;
        myRotation = aData.transform.rotation.eulerAngles;
        myScale = aData.transform.localScale;
        
        myStartPhases.x = aData.myStageOne.x;
        myStartPhases.y = aData.myStageTwo.x;
        myStartPhases.z = aData.myStageThree.x;

        myEndPhases.x = aData.myStageOne.y;
        myEndPhases.y = aData.myStageTwo.y;
        myEndPhases.z = aData.myStageThree.y;
    }
}
