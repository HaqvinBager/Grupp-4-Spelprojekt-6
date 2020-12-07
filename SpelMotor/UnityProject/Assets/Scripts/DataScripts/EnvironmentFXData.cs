using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public struct SEnvironmentFXData
{
    public int myInstanceID;
    public Vector3 myPosition;
    public Vector3 myRotation;
    public Vector3 myScale;
    public string myJsonName;

    public SEnvironmentFXData(EnvironmentFXSetup aData)
    {
        myInstanceID = aData.gameObject.GetInstanceID();
        myPosition = aData.transform.position;
        myRotation = aData.transform.rotation.eulerAngles;
        myScale = aData.transform.localScale;
        myJsonName = aData.environmentFXType.ToString();
    }
}