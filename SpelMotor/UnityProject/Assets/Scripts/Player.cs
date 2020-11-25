using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[ExecuteAlways]
public class Player : MonoBehaviour
{
    [HideInInspector]
    public PlayerData playerData;

    private void Update()
    {
        if(transform.hasChanged == true) {
            FindObjectOfType<CameraSetup>().OnValidate();
        }
    }
}
