using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[ExecuteAlways]
public class EventSetup : MonoBehaviour
{
    [HideInInspector]
    public Vector2 myColliderData;

    public Event myEvent;
    private void Update()
    {
        if (TryGetComponent<BoxCollider>(out BoxCollider collider))
        {
            myColliderData.x = collider.size.x;
            myColliderData.y = collider.size.z;
        }
    }

}
