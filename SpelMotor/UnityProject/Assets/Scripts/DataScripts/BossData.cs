using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEngine;

public struct BossData 
{
    public int myInstanceID;
    public int myModelIndex;
    public Vector3 myPosition;
    public Vector3 myRotation;
    public Vector3 myScale;
    public Vector2 myStageOne;
    public Vector2 myStageTwo;
    public Vector2 myStageThree;

    public BossData(BossSetup aData, int aModelIndex)
    {
        myInstanceID = aData.gameObject.GetInstanceID();
        myModelIndex = aModelIndex;
        myPosition = aData.transform.position;
        myRotation = aData.transform.rotation.eulerAngles;
        myScale = aData.transform.localScale;
        myStageOne = aData.myStageOne;
        myStageTwo = aData.myStageTwo;
        myStageThree = aData.myStageThree;
    }
}