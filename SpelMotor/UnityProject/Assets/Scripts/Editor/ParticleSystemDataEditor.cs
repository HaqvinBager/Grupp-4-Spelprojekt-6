using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
[CustomEditor(typeof(ParticleSystemData))]
public class ParticleSystemDataEditor : Editor
{
    public override void OnInspectorGUI()
    {
        serializedObject.Update();
        base.OnInspectorGUI();

        string[] paths = JsonFinder.GetJsonPaths("VFXData_").ToArray();
        int choice = serializedObject.FindProperty("myTexturePathIndex").intValue;
        choice = EditorGUILayout.Popup("Texture", choice, paths);
        serializedObject.FindProperty("myTexturePathIndex").intValue = choice;

        serializedObject.ApplyModifiedProperties();
    }
}
