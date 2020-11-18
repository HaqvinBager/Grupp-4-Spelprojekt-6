using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Experimental.GlobalIllumination;

public struct DirectionalLightData
{
    public DirectionalLightData(Light aDirectionalLight)
    {
        myDirectionX = -aDirectionalLight.transform.forward.x;
        myDirectionY = -aDirectionalLight.transform.forward.y;
        myDirectionZ = -aDirectionalLight.transform.forward.z;
        myColorR = aDirectionalLight.color.r;
        myColorG = aDirectionalLight.color.g;
        myColorB = aDirectionalLight.color.b;
        myIntensity = aDirectionalLight.intensity;
    }
    public float myDirectionX;
    public float myDirectionY;
    public float myDirectionZ;
    public float myColorR;
    public float myColorG;
    public float myColorB;
    public float myIntensity;
}
