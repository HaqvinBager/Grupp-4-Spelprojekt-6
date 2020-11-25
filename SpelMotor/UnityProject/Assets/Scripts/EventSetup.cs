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
       myColliderData.x = gameObject.GetComponent<BoxCollider>().size.x;
       myColliderData.y = gameObject.GetComponent<BoxCollider>().size.z;
    }
    
}
