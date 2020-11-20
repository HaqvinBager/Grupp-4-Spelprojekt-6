using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[Serializable]
public struct PlayerData 
{
    public PlayerData(Player player, int aModelIndex)
    {
        myInstanceID = player.GetInstanceID();
        myPosition = player.transform.position;
        myRotation = player.transform.rotation.eulerAngles;
        myScale = player.transform.localScale;
        myModelIndex = aModelIndex;
    }

    public int myInstanceID;
    public Vector3 myPosition;
    public Vector3 myRotation;
    public Vector3 myScale;
    public int myModelIndex;
}
