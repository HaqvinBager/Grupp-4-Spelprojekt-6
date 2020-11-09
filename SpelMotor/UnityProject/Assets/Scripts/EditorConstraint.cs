using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[ExecuteInEditMode]
public class EditorConstraint : MonoBehaviour
{
    
    [Range(0.1f, 2.0f)]
    public float myScale = 1.0f;


    private void Update()
    {
        
        if(!Mathf.Approximately(myScale, transform.localScale.x) || !Mathf.Approximately(myScale, transform.localScale.y) || !Mathf.Approximately(myScale, transform.localScale.z))
        {
            transform.localScale = new Vector3(myScale, myScale, myScale);
        }
    }
}
