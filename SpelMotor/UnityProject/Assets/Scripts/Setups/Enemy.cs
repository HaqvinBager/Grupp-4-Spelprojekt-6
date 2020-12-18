using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Enemy : MonoBehaviour
{
    [Range(15.0f, 100.0f)]   public float health;
    public float damage;
    public float moveSpeed;
    public float damageCooldown;
    public float visionRange;
    public float attackRange;

}
