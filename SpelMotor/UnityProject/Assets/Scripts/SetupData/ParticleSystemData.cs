using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[CreateAssetMenu(menuName ="FX/ParticleSystem")]
public class ParticleSystemData : ScriptableObject
{
    public int myMaxNumberOfParticles;
    public float myDelay;
    public float myDuration;
    public float mySpawnDuration;
    public float myLifeTime;
    public float mySpeed;
    public Color myStartColor;
    public Color myEndColor;
    public Vector2 myStartEndSize;
    public Vector3 myOffsetPosition;
    public Vector3 myStartDirection;
    public Vector3 myEndDirection;
    public Vector2 myLiftimeLowerUpperBounds;
    public Vector2 mySpeedLowerUpperBounds;
    public Vector3 myDirectionLowerBound;
    public Vector3 myDirectionUpperBound;
    [HideInInspector] public int myTexturePathIndex;
}
