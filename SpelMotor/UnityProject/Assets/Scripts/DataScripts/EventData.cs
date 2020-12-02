﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum Event
{
    LoadLevel = 6
}

public struct EventData
{
    public Vector3 myPosition;
    public Vector2 myColliderData;
    public int myEvent;
    public string myEventString;
    public EventData(EventSetup eventSetup)
    {
        myPosition = eventSetup.transform.position;
        myColliderData.x = eventSetup.myColliderData.x;
        myColliderData.y = eventSetup.myColliderData.y;
        myEvent = (int)eventSetup.myEvent;
        myEventString = eventSetup.myEventString;
    }
}
