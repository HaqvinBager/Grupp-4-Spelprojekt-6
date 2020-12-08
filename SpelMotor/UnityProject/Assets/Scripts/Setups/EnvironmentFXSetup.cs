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
    }

}
