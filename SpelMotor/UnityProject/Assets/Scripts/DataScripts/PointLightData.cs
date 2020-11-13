using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public struct PointLightData
{
    public PointLightData(Light light)
    {
        myInstanceID = light.gameObject.GetInstanceID();
        myPosition = light.transform.position;
        myRange = light.range;
        myColor = new Vector3(light.color.r, light.color.g, light.color.b);
        myIntensity = light.intensity;          
    }
    public int myInstanceID;
    public Vector3 myPosition;
    public float myRange;
    public Vector3 myColor;
    public float myIntensity;
}
