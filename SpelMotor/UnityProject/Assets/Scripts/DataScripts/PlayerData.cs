using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[Serializable]
public struct PlayerData 
{
    public PlayerData(Player player, int aModelIndex)
    {
        myInstanceID = player.gameObject.GetInstanceID();
        myPosition = player.transform.position;

        Vector3 eulerAngles = player.transform.eulerAngles;
        myRotation.x = (-eulerAngles.x) - 360.0f;
        myRotation.y = eulerAngles.y + 180.0f;
        myRotation.z = (-eulerAngles.z) - 360.0f;

        myScale = player.transform.localScale;
        myModelIndex = aModelIndex;
    }

    public int myInstanceID;
    public Vector3 myPosition;
    public Vector3 myRotation;
    public Vector3 myScale;
    public int myModelIndex;
}
