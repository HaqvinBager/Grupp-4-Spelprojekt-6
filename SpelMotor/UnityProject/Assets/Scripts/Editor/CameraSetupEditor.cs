using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

[CustomEditor(typeof(CameraSetup))]
public class CameraSetupEditor : Editor
{

    public override void OnInspectorGUI()
    {
        base.OnInspectorGUI();

        if(FindObjectOfType<Player>() != null)
        {
            if(GUILayout.Button("Center Camera On Player"))
            {
                CameraSetup cameraSetup = target as CameraSetup;
                cameraSetup.LookAtPlayer();
            }
        }
    }
}
