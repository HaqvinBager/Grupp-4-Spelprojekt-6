using UnityEngine;
using UnityEditor;
using System;
using System.Diagnostics;
using System.IO;
using System.Text;

[InitializeOnLoad]
public static class IronWrought
{
    static IronWrought()
    {
        EditorApplication.playModeStateChanged += PlayStateChanged;
    }

    private static void PlayStateChanged(PlayModeStateChange aState)
    {
        if (aState != PlayModeStateChange.ExitingEditMode)
            return;

        if (GetCreateLauncherSetting(out LaunchModeSetting setting))
        {
            if (setting.launchMode == ELaunchMode.UnityEditor)
                return;
            else
                EditorApplication.isPlaying = false;


            SLauncherData data = setting.GetLauncherData(setting.launchMode);

            if (setting.bakeNavMeshBeforeExport)
            {
                UnityEditor.AI.NavMeshBuilder.BuildNavMesh();
            }

            if (setting.exportBin)
            {           
                BinaryExporter.DoExportBinary();
                CreateDebugLevelJson(data);
            }

            ProcessStartInfo info = new ProcessStartInfo(data.myExePath);
            info.WorkingDirectory = data.myWorkingDirectoryPath;
            Process.Start(info);
        }
    }

    private static void CreateDebugLevelJson(SLauncherData data)
    {
        using (System.IO.StreamWriter file = new System.IO.StreamWriter(data.myWorkingDirectoryPath + "Levels\\DebugLevel.json"))
        {
            string sceneName = UnityEditor.SceneManagement.EditorSceneManager.GetActiveScene().name;
            int sceneIndex = ConvertSceneToIndex(sceneName);
            file.WriteLine("{");

            file.Write("\"LevelName\" : ");
            file.Write("\"");
            file.Write(sceneName);
            file.WriteLine("\",");
            file.Write("\"LevelIndex\" : ");
            file.WriteLine(sceneIndex);
            file.WriteLine("}");
        }
    }

    private static bool GetCreateLauncherSetting(out LaunchModeSetting setting)
    {
        setting = AssetDatabase.LoadAssetAtPath<LaunchModeSetting>("Assets\\LaunchModeSetting.asset");
        if (setting == null)
        {
            setting = ScriptableObject.CreateInstance<LaunchModeSetting>();
            AssetDatabase.CreateAsset(setting, "Assets\\LaunchModeSetting.asset");
            AssetDatabase.Refresh();
            UnityEngine.Debug.LogWarning("Could not Find LaunchModeSetting Asset. We have created one for you ;) Please Try again!");
            EditorGUIUtility.PingObject(setting);
            EditorApplication.isPlaying = false;
        }
        return setting != null;
    }


    private static int ConvertSceneToIndex(string name)
    {
        switch (name)
        {
            case "SampleScene":
                return 0;
            case "Dungeon":
                return 1;
            case "Gardens":
                return 2;
            case "Castle":
                return 3;
            case "NavTest":
                return 4;
            default:
                return 1;
        }
    }
}


//if (setting.useEditorCameraView)
//{
//    CameraSetup cam = GameObject.FindObjectOfType<CameraSetup>();
//    cam.startInCameraMode = CameraMode.FreeCam;
//    CameraSetup cameraClone = (CameraSetup) PrefabUtility.InstantiatePrefab(PrefabUtility.GetCorrespondingObjectFromSource(cam));
//    cameraClone.transform.position = cam.transform.position;
//    cameraClone.transform.rotation = cam.transform.rotation;
//    cameraClone.GetComponent<Camera>().fieldOfView = cam.GetComponent<Camera>().fieldOfView;
//    cameraClone.GetComponent<Camera>().nearClipPlane = cam.GetComponent<Camera>().nearClipPlane;
//    cameraClone.GetComponent<Camera>().farClipPlane = cam.GetComponent<Camera>().farClipPlane;

//    Camera[] editorCameras = SceneView.GetAllSceneCameras();                 
//    cam.transform.position = editorCameras[0].transform.position;
//    cam.transform.rotation = editorCameras[0].transform.rotation;
//    cam.GetComponent<Camera>().fieldOfView = editorCameras[0].fieldOfView;
//    cam.GetComponent<Camera>().nearClipPlane = editorCameras[0].nearClipPlane;
//    cam.GetComponent<Camera>().farClipPlane = editorCameras[0].farClipPlane;
// }