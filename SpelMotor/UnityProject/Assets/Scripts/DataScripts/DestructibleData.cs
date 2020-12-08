using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public struct DestructibleData
{
    public Vector3 myPosition;
    public Vector3 myRotation;
    public Vector3 myScale;
    public int myModelIndex;

    public DestructibleData(Destructible destructible, int aModelIndex)
    {
        myPosition = destructible.transform.position;

        Vector3 eulerAngles = destructible.transform.eulerAngles;
        myRotation.x = (-eulerAngles.x) - 360.0f;
        myRotation.y = eulerAngles.y + 180.0f;
        myRotation.z = (-eulerAngles.z) - 360.0f;

        myScale = destructible.transform.localScale;
        myModelIndex = aModelIndex;
    }
}
