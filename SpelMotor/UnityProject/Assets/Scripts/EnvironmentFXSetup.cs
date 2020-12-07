using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum EnvironmentFXType
{
}

public class EnvironmentFXSetup : MonoBehaviour
{
    public EnvironmentFXType environmentFXType;

    private void OnValidate()
    {
        Debug.Log(environmentFXType.ToString());
    }

}
