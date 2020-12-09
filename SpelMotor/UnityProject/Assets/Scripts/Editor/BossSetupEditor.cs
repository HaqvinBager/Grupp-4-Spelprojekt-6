using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

[CustomEditor(typeof(BossSetup))]
public class BossSetupEditor : Editor
{
    float minLimit = 0.0f;
    float maxLimit = 100.0f;

    //SerializedProperty minFloat0;
    //SerializedProperty maxFloat0;

    //SerializedProperty[] myMinFloats;
    //SerializedProperty[] myMaxFloats;

    SerializedProperty[] myStageMinMaxProp;


    private void OnEnable()
    {
        //myMinFloats = new SerializedProperty[3];
        //myMinFloats[0] = serializedObject.FindProperty("myFirstStageStart");
        //myMinFloats[1] = serializedObject.FindProperty("mySecondStageStart");
        //myMinFloats[2] = serializedObject.FindProperty("myThirdStageStart");

        //myMaxFloats = new SerializedProperty[3];
        //myMaxFloats[0] = serializedObject.FindProperty("myFirstStageEnd");
        //myMaxFloats[1] = serializedObject.FindProperty("mySecondStageEnd");
        //myMaxFloats[2] = serializedObject.FindProperty("myThirdStageEnd");

        //SerializedProperty prop = serializedObject.FindProperty("myStageOne");

        myStageMinMaxProp = new SerializedProperty[3];
        myStageMinMaxProp[0] = serializedObject.FindProperty("myStageOne");
        myStageMinMaxProp[1] = serializedObject.FindProperty("myStageTwo");
        myStageMinMaxProp[2] = serializedObject.FindProperty("myStageThree");
    }

    public override void OnInspectorGUI()
    {
        base.OnInspectorGUI();

        serializedObject.Update();

        for(int i = 0; i < 3; ++i)
        {
            EditorGUILayout.LabelField("Stage " + i);
            EditorGUILayout.Vector2Field("Health in %", myStageMinMaxProp[i].vector2Value);

            float minVal = myStageMinMaxProp[i].vector2Value.x;
            float maxVal = myStageMinMaxProp[i].vector2Value.y;
            EditorGUILayout.MinMaxSlider(ref minVal, ref maxVal, 0, 100);
            myStageMinMaxProp[i].vector2Value = new Vector2(minVal, maxVal);

        }
        serializedObject.ApplyModifiedProperties();
    }

}
