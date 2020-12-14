using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

[CustomEditor(typeof(BossSetup))]
public class BossSetupEditor : Editor
{
    float minLimit = 0.0f;
    float maxLimit = 100.0f;

    SerializedProperty[] myStageMinMaxProp;


    private void OnEnable()
    {
        myStageMinMaxProp = new SerializedProperty[3];
        myStageMinMaxProp[0] = serializedObject.FindProperty("myStageOne");
        myStageMinMaxProp[1] = serializedObject.FindProperty("myStageTwo");
        myStageMinMaxProp[2] = serializedObject.FindProperty("myStageThree");
    }

    public override void OnInspectorGUI()
    {
        base.OnInspectorGUI();
       
        EditorGUILayout.Space();
        EditorGUILayout.Space();

        serializedObject.Update();


        for (int i = 0; i < 3; ++i)
        {
            EditorGUILayout.LabelField("Stage " + i);
            EditorGUILayout.Vector2Field("Health in %", myStageMinMaxProp[i].vector2Value);

            float minVal = myStageMinMaxProp[i].vector2Value.x;
            float maxVal = myStageMinMaxProp[i].vector2Value.y;
            EditorGUILayout.MinMaxSlider(ref minVal, ref maxVal, minLimit, maxLimit);
            myStageMinMaxProp[i].vector2Value = new Vector2(minVal, maxVal);

        }
        serializedObject.ApplyModifiedProperties();
    }
}
