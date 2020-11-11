using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;


public interface IBinaryWritable
{
    void WriteTo(BinaryWriter aBinaryWrite);
}


public struct GameObjectData : IBinaryWritable
{
    public int myInstanceID;
    public Vector3 myPosition;
    public Vector3 myRotation;
    public Vector3 myScale;
    public int myModelIndex;

    public GameObjectData(GameObject aGameObject)
    {
        myInstanceID = aGameObject.GetInstanceID();
        myPosition = aGameObject.transform.position;
        myRotation = aGameObject.transform.rotation.eulerAngles;
        myScale = aGameObject.transform.localScale;
        myModelIndex = 0;
    }

    public void WriteTo(BinaryWriter aBinaryWriter)
    {
        aBinaryWriter.Write(myInstanceID);

        aBinaryWriter.Write(myPosition.x);
        aBinaryWriter.Write(myPosition.y);
        aBinaryWriter.Write(myPosition.z);

        aBinaryWriter.Write(myRotation.x);
        aBinaryWriter.Write(myRotation.y);
        aBinaryWriter.Write(myRotation.z);

        aBinaryWriter.Write(myScale.x);
        aBinaryWriter.Write(myScale.y);
        aBinaryWriter.Write(myScale.z);

        aBinaryWriter.Write(myModelIndex);
    }
}
