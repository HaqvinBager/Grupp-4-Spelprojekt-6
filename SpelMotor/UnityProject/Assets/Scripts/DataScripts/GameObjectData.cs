using UnityEngine;

public struct PrefabInstanceData
{
    public int myInstanceID;
    public Vector3 myPosition;
    public Vector3 myRotation;
    public Vector3 myScale;
    public int myModelIndex;

    public PrefabInstanceData(GameObject aGameObject, int aModelIndex)
    {
        myInstanceID = aGameObject.GetInstanceID();
        myPosition = aGameObject.transform.position;

        Vector3 eulerAngles = aGameObject.transform.eulerAngles;
        myRotation.x = (-eulerAngles.x) - 360.0f;
        myRotation.y = eulerAngles.y + 180.0f;
        myRotation.z = (-eulerAngles.z) - 360.0f;

        myScale = aGameObject.transform.localScale;
        myModelIndex = aModelIndex;
    }
}
