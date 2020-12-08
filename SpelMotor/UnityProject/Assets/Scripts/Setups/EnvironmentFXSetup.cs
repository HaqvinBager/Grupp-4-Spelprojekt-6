using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;

public enum EnvironmentFXType
{
    Water,
    CrackDaughter,
    FogPlane
}

[System.Serializable]
public struct VFXData
{
    public string myJson;
    public bool useMe;
}

public class EnvironmentFXSetup : MonoBehaviour
{
    public EnvironmentFXType environmentFXType;

    public List<VFXData> myJsonPaths = new List<VFXData>();

    private string target_path = "..\\IronWrought\\Bin\\json\\";
    
    private void OnValidate()
    {
        JsonFinder.RefreshData(myJsonPaths, "VFXData_");
        //string[] paths = Directory.GetFiles(target_path, "*.json");
        ////myJsonPaths.Clear();
        //foreach (string vfxData in paths)
        //{
        //    int startIndex = vfxData.IndexOf("VFX");
        //    if(startIndex > 0)
        //    {
        //        bool foundNew = true;
        //        string jsonName = vfxData.Substring(startIndex, vfxData.Length - startIndex);
        //        if (jsonName.Contains("_UI"))
        //        {
        //            continue;
        //        }

        //        foreach(var data in myJsonPaths)
        //        {
        //            if (data.myJson.Equals(jsonName))
        //            {
        //                foundNew = false;
        //            }
        //        }
        //        if (foundNew)
        //        {
        //            myJsonPaths.Add(new VFXData {myJson = jsonName });             
        //        }
        //    }
        //}
        //Debug.Log(environmentFXType.ToString());
    }

}
