using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public struct SParticleFXData
{
    public int myInstanceID;
    public Vector3 myPosition;
    public Vector3 myRotation;
    public int myJsonListCount;
    public List<string> myJsonList;


    public SParticleFXData(ParticleFXSetup aData)
    {
        myJsonList = new List<string>();
        myInstanceID = aData.gameObject.GetInstanceID();
        foreach(var vfxData in aData.dataList)
        {
            if (vfxData.useMe)
            {
                myJsonList.Add(vfxData.myJson);
            }
        }
        myJsonListCount = myJsonList.Count;
        myPosition = aData.transform.position;
        myRotation = aData.transform.rotation.eulerAngles;
    }

}
