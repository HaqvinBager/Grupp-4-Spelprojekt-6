using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum CameraMode
{
    Player,
    FreeCam
}

public class CameraSetup : MonoBehaviour
{
    public CameraMode startInCameraMode;
    public KeyCode toggleFreeCamKey = KeyCode.F;

    [Range(5.0f, 50.0f)]
    public float freeCamMoveSpeed = 25.0f;
    public Vector3 offset;

    public void OnValidate()
    {
        if (startInCameraMode == CameraMode.Player)
        {
            if (Vector3.Distance(FindObjectOfType<Player>().transform.position + offset, transform.position) > 0.01f)
            {
                transform.position = FindObjectOfType<Player>().transform.position + offset;
            }
        }
    }
}


