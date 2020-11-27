using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public struct EnemyData
{
    public Vector3 myPosition;
    public Vector3 myRotation;
    public Vector3 myScale;
    public float myHealth;
    public float myDamage;
    public float myMoveSpeed;
    public float myVisionRange;
    public float myAttackRange;
    public int myModelIndex;
    public EnemyData (Enemy enemy, int aModelIndex)
    {
        myPosition = enemy.transform.position;

        Vector3 eulerAngles = enemy.transform.eulerAngles;
        myRotation.x = (-eulerAngles.x) - 360.0f;
        myRotation.y = eulerAngles.y + 180.0f;
        myRotation.z = (-eulerAngles.z) - 360.0f;

        myScale = enemy.transform.localScale;
        myHealth = enemy.health;
        myDamage = enemy.damage;
        myMoveSpeed = enemy.moveSpeed;
        myVisionRange = enemy.visionRange;
        myAttackRange = enemy.attackRange;
        myModelIndex = aModelIndex;
    }
}
