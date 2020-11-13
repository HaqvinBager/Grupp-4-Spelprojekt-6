using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public struct CameraData
{
    public CameraData(Camera aCamera) {
        myRotX = aCamera.transform.rotation.eulerAngles.x;
        myRotY = aCamera.transform.rotation.eulerAngles.y;
        myRotZ = aCamera.transform.rotation.eulerAngles.z;
        myPosX = aCamera.transform.position.x;
        myPosY = aCamera.transform.position.y;
        myPosZ = aCamera.transform.position.z;
        myFieldOfView = aCamera.fieldOfView;
    }
    public float myRotX;
    public float myRotY;
    public float myRotZ;
    public float myPosX;
    public float myPosY;
    public float myPosZ;
    public float myFieldOfView;
}
