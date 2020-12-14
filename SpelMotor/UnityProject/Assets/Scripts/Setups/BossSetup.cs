using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BossSetup : MonoBehaviour
{
    public float health;
    public float damage;
    public float moveSpeed;
    public float damageCooldown;
    public float visionRange;
    public float attackRange;

    [Space(10)]
    //Editor Script Makes these show up as MinMax Sliders
    [HideInInspector] public Vector2 myStageOne;
    [HideInInspector] public Vector2 myStageTwo;
    [HideInInspector] public Vector2 myStageThree;
}
