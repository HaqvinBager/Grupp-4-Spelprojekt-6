using UnityEngine;
using UnityEditor;
using System.Collections;
using System.Collections.Generic;
using System;
using System.IO;
using UnityEditor.SceneManagement;
using UnityEngine.SceneManagement;

public class BinaryExporter
{
    [MenuItem("Tools/Export all BIN #_y")]
    private static void DoExportBinary()
    {
        string target_path = "..\\IronWrought\\Bin\\Levels\\";
        if (!System.IO.Directory.Exists(target_path))
        {
            System.IO.Directory.CreateDirectory(target_path);
        }

        string scene_name = SceneManager.GetActiveScene().name;

        // Save paths
        Dictionary<string, string> filter = new Dictionary<string, string>();
      
        GameObject[] allModels = UnityEngine.Object.FindObjectsOfType<GameObject>();
        foreach (GameObject go in allModels)
        {
            if (go.transform.childCount == 0)
                continue;
            GameObject child = go.transform.GetChild(0).gameObject;

            MeshFilter mesh = child.GetComponent<MeshFilter>();
            if (mesh != null)
            {
                    
                string path = AssetDatabase.GetAssetPath(PrefabUtility.GetCorrespondingObjectFromOriginalSource(mesh));
                filter[path] = path;
                //
            }
        }
 

        using (System.IO.StreamWriter file = new System.IO.StreamWriter(target_path + scene_name + "_bin_modelPaths.txt"))
        {
            foreach (KeyValuePair<string, string> entry in filter)
            {
                file.WriteLine(entry.Key);
                // do something with entry.Value or entry.Key
            }
        }


        BinaryWriter bw;
        bw = new BinaryWriter(new FileStream(target_path + scene_name + "_bin.bin", FileMode.Create));

        GameObject[] allObjects = UnityEngine.Object.FindObjectsOfType<GameObject>();

        int totalAssetsSizeToExport = 0;
        foreach (GameObject go in allObjects)
        {
            if (go.transform.childCount == 0)
                continue;

            GameObject child = go.transform.GetChild(0).gameObject;
            MeshFilter mesh = child.GetComponent<MeshFilter>();
            if (mesh != null)
            {
                totalAssetsSizeToExport++;
            }
        }


        CameraSettings cameraSetting = new CameraSettings(UnityEngine.Object.FindObjectOfType<Camera>());
        bw.Write(cameraSetting.myRotX);
        bw.Write(cameraSetting.myRotY);
        bw.Write(cameraSetting.myRotZ);
        bw.Write(cameraSetting.myPosX);
        bw.Write(cameraSetting.myPosY);
        bw.Write(cameraSetting.myPosZ);
        bw.Write(cameraSetting.myFieldOfView);

        Light[] lights = UnityEngine.Object.FindObjectsOfType<Light>();
        Light directionalLight = null;
        foreach (Light light in lights)
        {
            if(light.type == LightType.Directional)
            {
                directionalLight = light;
            }
        }
        if(directionalLight == null)
        {
            Debug.LogError("No directionlight found! Please add one before you export!");
            return;
        }

        DirectionalLightSettings directionalLightSettings = new DirectionalLightSettings(directionalLight);
        bw.Write(directionalLightSettings.myDirectionX);
        bw.Write(directionalLightSettings.myDirectionY);
        bw.Write(directionalLightSettings.myDirectionZ);
        bw.Write(directionalLightSettings.myColorR);
        bw.Write(directionalLightSettings.myColorG);
        bw.Write(directionalLightSettings.myColorB);
        bw.Write(directionalLightSettings.myIntensity);

        bw.Write(totalAssetsSizeToExport);
        foreach (GameObject go in allObjects)
        {
            if (go.transform.childCount == 0)
                continue;

            GameObject child = go.transform.GetChild(0).gameObject;

            MeshFilter mesh = child.GetComponent<MeshFilter>();
            if(mesh != null)
            {
                int index = Getindex(PrefabUtility.GetCorrespondingObjectFromOriginalSource<GameObject>(child), filter);
                //Debug.Log(index);
                GameObjectData gameObjectData = new GameObjectData(child);
                //gameObjectData.myModelIndex = index;
                gameObjectData.WriteTo(bw);

                //bw.Write(go.GetInstanceID());
                //bw.Write(-go.transform.rotation.eulerAngles.x - 360.0f);
                //bw.Write(go.transform.rotation.eulerAngles.y + 180.0f);
                //bw.Write(-go.transform.rotation.eulerAngles.z - 360.0f);
                //bw.Write(go.transform.position.x);
                //bw.Write(go.transform.position.y);
                //bw.Write(go.transform.position.z);

                //bw.Write(go.transform.localScale.x);
                //bw.Write(go.transform.localScale.y);
                //bw.Write(go.transform.localScale.z);
                //bw.Write(index);
            }
        }
        bw.Close();
    }

    private static int Getindex(GameObject go, Dictionary<string, string> aDic)
    {
        int index = -1;
        foreach (KeyValuePair<string, string> entry in aDic)
        {
            index++;

            string path = AssetDatabase.GetAssetPath(PrefabUtility.GetCorrespondingObjectFromOriginalSource(go));

            if (entry.Value == path)
            {
                break;
            }

            // do something with entry.Value or entry.Key
        }
        return index;
    }

}