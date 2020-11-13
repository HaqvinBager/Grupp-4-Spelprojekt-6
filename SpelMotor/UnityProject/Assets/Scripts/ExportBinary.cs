﻿using UnityEngine;
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

        CameraData cameraData = new CameraData(UnityEngine.Object.FindObjectOfType<Camera>());
        bw.Write(cameraData.myRotX);
        bw.Write(cameraData.myRotY);
        bw.Write(cameraData.myRotZ);
        bw.Write(cameraData.myPosX);
        bw.Write(cameraData.myPosY);
        bw.Write(cameraData.myPosZ);
        bw.Write(cameraData.myFieldOfView);

        Light[] lights = UnityEngine.Object.FindObjectsOfType<Light>();
        List<Light> pointLights = new List<Light>();
        Light directionalLight = null;
        foreach (Light light in lights)
        {
            if(light.type == LightType.Directional)
            {
                directionalLight = light;
            }else if(light.type == LightType.Point)
            {
                pointLights.Add(light);
            }

        }
        if(directionalLight == null)
        {
            Debug.LogError("No directionlight found! Please add one before you export!");
            return;
        }
        DirectionalLightData directionalLightData = new DirectionalLightData(directionalLight);
        bw.Write(directionalLightData.myDirectionX);
        bw.Write(directionalLightData.myDirectionY);
        bw.Write(directionalLightData.myDirectionZ);
        bw.Write(directionalLightData.myColorR);
        bw.Write(directionalLightData.myColorG);
        bw.Write(directionalLightData.myColorB);
        bw.Write(directionalLightData.myIntensity);


        bw.Write(pointLights.Count);
        foreach(Light pointLight in pointLights)
        {
            PointLightData pointLightData = new PointLightData(pointLight);
            bw.Write(pointLightData.myInstanceID);
            bw.Write(pointLightData.myPosition.x);
            bw.Write(pointLightData.myPosition.y);
            bw.Write(pointLightData.myPosition.z);
            bw.Write(pointLightData.myRange);
            bw.Write(pointLightData.myColor.x);
            bw.Write(pointLightData.myColor.y);
            bw.Write(pointLightData.myColor.z);
            bw.Write(pointLightData.myIntensity);
        }



        if (UnityEngine.Object.FindObjectOfType<Player>() == null)
        {
            Debug.LogWarning("No Player Found! Please add one before you export!");
            return;
        }
        
        GameObject playerGameObject = UnityEngine.Object.FindObjectOfType<Player>().gameObject;
    
        PlayerData playerData = new PlayerData(playerGameObject.GetComponent<Player>());
        bw.Write(playerData.myInstanceID);
        bw.Write(playerData.myRotation.z);
        bw.Write(playerData.myRotation.z);
        bw.Write(playerData.myRotation.z);
        bw.Write(playerData.myPosition.x);
        bw.Write(playerData.myPosition.y);
        bw.Write(playerData.myPosition.z);
        bw.Write(playerData.myScale.z);
        bw.Write(playerData.myScale.z);
        bw.Write(playerData.myScale.z);
        //Add more PlayerData her :) //Axel & Alexander //2020-11-12
        int playerModelIndex = Getindex(PrefabUtility.GetCorrespondingObjectFromOriginalSource<GameObject>(playerGameObject.transform.GetChild(0).gameObject), filter);
        bw.Write(playerModelIndex);

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
                //GameObjectData gameObjectData = new GameObjectData(child);
                ////gameObjectData.myModelIndex = index;
                //gameObjectData.WriteTo(bw);

                bw.Write(go.GetInstanceID());
                bw.Write(-go.transform.rotation.eulerAngles.x - 360.0f);
                bw.Write(go.transform.rotation.eulerAngles.y + 180.0f);
                bw.Write(-go.transform.rotation.eulerAngles.z - 360.0f);
                bw.Write(go.transform.position.x);
                bw.Write(go.transform.position.y);
                bw.Write(go.transform.position.z);

                bw.Write(go.transform.localScale.x);
                bw.Write(go.transform.localScale.y);
                bw.Write(go.transform.localScale.z);
                bw.Write(index);
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