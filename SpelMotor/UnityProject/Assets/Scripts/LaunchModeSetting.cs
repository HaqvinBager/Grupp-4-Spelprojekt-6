using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

public enum ELaunchMode
{
    Debug,
    Release,
    ModelViewerDebug,
    ModelViewerRelease,
    UnityEditor
}

//This exists to make things Futureproof
[System.Serializable]
public struct SLauncherData
{
    [ReadOnly] public string myExePath;
    [ReadOnly] public string myWorkingDirectoryPath;
    [ReadOnly] public ELaunchMode myLaunchMode;
}


[CreateAssetMenu(menuName = "Launcher Setting")]
public class LaunchModeSetting : ScriptableObject
{
    [Header("Select Launch Mode")]
    [Tooltip("Recommended\nLevel Designer: Release\nGraphics: ModelViewer Release\nProgrammer: Debug\nTo Play inside Unity choose UnityEditor")]
    public ELaunchMode launchMode;

    [Header("Build NavMesh On Play")]
    public bool bakeNavMeshBeforeExport = true;

    [Header("Export Bin On Play")]
    public bool exportBin = true;


    [Space(20)]
    public List<SLauncherData> mylauncherData = new List<SLauncherData>() {
        new SLauncherData{ 
            myExePath = "Launcher_Debug.exe", 
            myWorkingDirectoryPath = "..\\IronWrought\\Bin\\", 
            myLaunchMode = ELaunchMode.Debug 
        },
        new SLauncherData{
            myExePath = "Launcher_Release.exe",
            myWorkingDirectoryPath = "..\\IronWrought\\Bin\\",
            myLaunchMode = ELaunchMode.Release
        },
        new SLauncherData{
            myExePath = "ModelViewer_Debug.exe",
            myWorkingDirectoryPath = "..\\IronWrought\\Bin\\",
            myLaunchMode = ELaunchMode.ModelViewerDebug
        },
        new SLauncherData{
            myExePath = "ModelViewer_Release.exe",
            myWorkingDirectoryPath = "..\\IronWrought\\Bin\\",
            myLaunchMode = ELaunchMode.ModelViewerRelease
        },
        new SLauncherData{
            myExePath = "",
            myWorkingDirectoryPath = "",
            myLaunchMode = ELaunchMode.UnityEditor
        },
    };

    public SLauncherData GetLauncherData(ELaunchMode aLaunchMode)
    {
        return mylauncherData.Find(e => e.myLaunchMode == aLaunchMode);
    }
}

#region Read Only Attribute (Found here: https://www.patrykgalach.com/2020/01/20/readonly-attribute-in-unity-editor/)
public class ReadOnlyAttribute : PropertyAttribute { }
/// <summary>
/// This class contain custom drawer for ReadOnly attribute.
/// </summary>
[CustomPropertyDrawer(typeof(ReadOnlyAttribute))]
public class ReadOnlyDrawer : PropertyDrawer
{
    /// <summary>
    /// Unity method for drawing GUI in Editor
    /// </summary>
    /// <param name="position">Position.</param>
    /// <param name="property">Property.</param>
    /// <param name="label">Label.</param>
    public override void OnGUI(Rect position, SerializedProperty property, GUIContent label)
    {
        // Saving previous GUI enabled value
        var previousGUIState = GUI.enabled;
        // Disabling edit for property
        GUI.enabled = false;
        // Drawing Property
        EditorGUI.PropertyField(position, property, label);
        // Setting old GUI enabled value
        GUI.enabled = previousGUIState;
    }
}
#endregion