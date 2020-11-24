using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public struct CameraData
{
    public float myPosX;
    public float myPosY;
    public float myPosZ;
    public float myRotX;
    public float myRotY;
    public float myRotZ;
    public float myFieldOfView;
    public float myStartInCameraMode;
    public float myToggleFreeCamKey;
    public float myFreeCamMoveSpeed;
    public Vector3 offset;

    public CameraData(Camera aCamera) {
        myRotX = aCamera.transform.rotation.eulerAngles.x;
        myRotY = aCamera.transform.rotation.eulerAngles.y;
        myRotZ = aCamera.transform.rotation.eulerAngles.z;
        myPosX = aCamera.transform.position.x;
        myPosY = aCamera.transform.position.y;
        myPosZ = aCamera.transform.position.z;
        myFieldOfView = aCamera.fieldOfView;
        if(aCamera.TryGetComponent<CameraSetup>(out CameraSetup setup))
        {
            float setupMode = (float)setup.startInCameraMode;
            myStartInCameraMode = setupMode;
            float value = (float)setup.toggleFreeCamKey;
            myToggleFreeCamKey = value;

            myFreeCamMoveSpeed = setup.freeCamMoveSpeed;
            offset = setup.offset;
        }
        else
        {
            Debug.LogWarning("Camera Prefab is Missing. Please add the Camera Prefab instead!");
            myStartInCameraMode = (float)CameraMode.FreeCam;
            myToggleFreeCamKey = (float)'f';
            myFreeCamMoveSpeed = 25.0f;
            offset = Vector3.zero;
        }
    }
}
