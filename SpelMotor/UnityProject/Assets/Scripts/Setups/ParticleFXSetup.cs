using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;

public class ParticleFXSetup : MonoBehaviour
{
    public string myParticleSuffix = "PD_";
    public List<VFXData> dataList = new List<VFXData>();

    private void OnValidate()
    {
        JsonFinder.RefreshData(dataList, myParticleSuffix);
    }
}