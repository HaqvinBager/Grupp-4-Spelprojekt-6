﻿using UnityEngine;
using UnityEditor;
using System.Collections;
using System.Collections.Generic;
using UnityEditor.SceneManagement;
using UnityEngine.SceneManagement;
using System.Runtime.InteropServices;
using UnityEditor.Animations;

public class MenuItems : MonoBehaviour
{
    // REMINDER: add changes made here to Binary export script. -> Slightly more difficult

    [MenuItem("Tools/Export all #s")]
    private static void NewMenuOption()
    {
        string target_path = "..\\IronWrought\\Bin\\Levels\\";
        if (!System.IO.Directory.Exists(target_path))
        {
            System.IO.Directory.CreateDirectory(target_path);
        }

        string scene_name = SceneManager.GetActiveScene().name;
        // its actually just a json file. Named .txt.
        using (System.IO.StreamWriter file = new System.IO.StreamWriter(target_path + scene_name + "_exportedLevelASCII.txt"))
        {
            // Gets all root objects in the scene (i.e children are not included)
            GameObject[] rootObjects = EditorSceneManager.GetActiveScene().GetRootGameObjects();

            List<GameObject> gameObjectsInScene = new List<GameObject>();
            foreach(GameObject go in rootObjects)
            {
                gameObjectsInScene.Add(go);
            }

            // Future additions/ changes, suggestion. Able to further separate
            //GameObject[] allObjects       = UnityEngine.Object.FindObjectsOfType<GameObject>();// FBX Transforms
            //Light[] allLights             = UnityEngine.Object.FindObjectsOfType<Light>();
            //Collider[] allColliders       = UnityEngine.Object.FindObjectsOfType<Collider>();

            // Can use tags 
            //if (go.CompareTag("Export"))// Change to check if root has child that is mesh. But grab 

            ListContainer objects = new ListContainer();
            objects.dataList = new List<ObjectData>();
            foreach (GameObject go in gameObjectsInScene)
            {
                if (go.transform.childCount > 0 && PrefabUtility.GetPrefabAssetType(go) != PrefabAssetType.Regular)
                {
                    print(go.transform.childCount);
                    for (int i = 0; i < go.transform.childCount; ++i)
                    {
                        ObjectData savingObject = new ObjectData();
                        Transform child = go.transform.GetChild(i);
                        // Get desired data from root objects
                        print("child: " + i);
                        // Transforms
                        /// the adjustments made to myRotXYZ is to make it correct in engine. There is most likely a cause to this. This should be looked over
                        savingObject.myRotX = -child.transform.rotation.eulerAngles.x - 360.0f;//Eueler is in Degrees?
                        savingObject.myRotY = child.transform.rotation.eulerAngles.y + 180.0f;
                        savingObject.myRotZ = -child.transform.rotation.eulerAngles.z - 360.0f;

                        savingObject.myPosX = child.transform.position.x;
                        savingObject.myPosY = child.transform.position.y;
                        savingObject.myPosZ = child.transform.position.z;
                        savingObject.myScaleX = child.transform.localScale.x;
                        savingObject.myScaleY = child.transform.localScale.y;
                        savingObject.myScaleZ = child.transform.localScale.z;

                        // Asset name
                        savingObject.myName = child.name;

                        // Is it a prefab?
                        if (PrefabUtility.GetPrefabAssetType(child) == PrefabAssetType.Regular)
                        {
                            //print(go.ToString() + " is a prefab");

                            // Check children for meshrenderer to find FBX object
                            if (child.transform.childCount > 0)
                            {
                                for (int y = 0; y < child.transform.childCount; ++y)
                                {
                                    var prefabchild = child.transform.GetChild(y);
                                    if (prefabchild.gameObject.GetComponentInChildren<MeshRenderer>())
                                    {
                                        savingObject.myPath = AssetDatabase.GetAssetPath(PrefabUtility.GetCorrespondingObjectFromOriginalSource(prefabchild));
                                    }
                                }
                            }
                            // Add prefab instance CharacterData if it has the appropriate component (PH is Player)
                            if (child.GetComponent<CharacterData>() != null)//Player.cs is placeholder
                            {
                                savingObject.myCharacterData = new CharacterDataStruct();
                                savingObject.myCharacterData.myName = child.GetComponent<CharacterData>().myName;
                                savingObject.myCharacterData.myHealth = child.GetComponent<CharacterData>().myHealth;
                                savingObject.myCharacterData.myDamage = child.GetComponent<CharacterData>().myDamage;
                                savingObject.myCharacterData.myAttackSpeed = child.GetComponent<CharacterData>().myAttackSpeed;
                                savingObject.myCharacterData.myMoveSpeed = child.GetComponent<CharacterData>().myMoveSpeed;
                                savingObject.myCharacterData.myCastSpeed = child.GetComponent<CharacterData>().myCastSpeed;
                            }
                        }
                        else // it is not a prefab: bare-FBX, Light or Camera, e.g.
                        {
                            savingObject.myPath = AssetDatabase.GetAssetPath(PrefabUtility.GetCorrespondingObjectFromSource(child));
                        }

                        if (child.GetComponent<Collider>())
                        {
                            // Int or String? Int -> Enum Clas in C++ faster comparison. String might be fine during load.
                            // UnityEngine.collider, UnityEngine. ends on index 12 
                            savingObject.myColliderData = new ColliderData();
                            savingObject.myColliderData.myColliderType = child.GetComponent<Collider>().GetType().ToString().Remove(0, 12);
                            print(savingObject.myColliderData.myColliderType);
                            if (savingObject.myColliderData.myColliderType == "BoxCollider")
                            {
                                print("Getting BoxCollider Data");
                                savingObject.myColliderData.mySizeX = child.GetComponent<BoxCollider>().size.x;
                                savingObject.myColliderData.mySizeY = child.GetComponent<BoxCollider>().size.y;
                                savingObject.myColliderData.mySizeZ = child.GetComponent<BoxCollider>().size.z;
                                savingObject.myColliderData.myCenterX = child.GetComponent<BoxCollider>().center.x;
                                savingObject.myColliderData.myCenterY = child.GetComponent<BoxCollider>().center.y;
                                savingObject.myColliderData.myCenterZ = child.GetComponent<BoxCollider>().center.z;

                            }
                            else if (savingObject.myColliderData.myColliderType == "SphereCollider")
                            {
                                print("Getting SphereCollider Data");
                                savingObject.myColliderData.myRadius = child.GetComponent<SphereCollider>().radius;
                                savingObject.myColliderData.myCenterX = child.GetComponent<SphereCollider>().center.x;
                                savingObject.myColliderData.myCenterY = child.GetComponent<SphereCollider>().center.y;
                                savingObject.myColliderData.myCenterZ = child.GetComponent<SphereCollider>().center.z;
                            }
                        }

                        objects.dataList.Add(savingObject);
                    }
                }
                else
                {
                    ObjectData savingObject = new ObjectData();
                    // Get desired data from root objects
                    // ObjectData savingObject = new ObjectData();

                    // Transforms
                    /// the adjustments made to myRotXYZ is to make it correct in engine. There is most likely a cause to this. This should be looked over
                    savingObject.myRotX = -go.transform.rotation.eulerAngles.x - 360.0f;//Eueler is in Degrees?
                    savingObject.myRotY = go.transform.rotation.eulerAngles.y + 180.0f;
                    savingObject.myRotZ = -go.transform.rotation.eulerAngles.z - 360.0f;

                    savingObject.myPosX = go.transform.position.x;
                    savingObject.myPosY = go.transform.position.y;
                    savingObject.myPosZ = go.transform.position.z;
                    savingObject.myScaleX = go.transform.localScale.x;
                    savingObject.myScaleY = go.transform.localScale.y;
                    savingObject.myScaleZ = go.transform.localScale.z;

                    // Asset name
                    savingObject.myName = go.name;

                    // Is it a prefab?
                    if (PrefabUtility.GetPrefabAssetType(go) == PrefabAssetType.Regular)
                    {
                        //print(go.ToString() + " is a prefab");

                        // Check children for meshrenderer to find FBX object
                        if (go.transform.childCount > 0)
                        {
                            for (int i = 0; i < go.transform.childCount; ++i)
                            {
                                var child = go.transform.GetChild(i);
                                if (child.gameObject.GetComponentInChildren<MeshRenderer>())
                                {
                                    savingObject.myPath = AssetDatabase.GetAssetPath(PrefabUtility.GetCorrespondingObjectFromOriginalSource(child));
                                }
                            }
                        }
                        // Add prefab instance CharacterData if it has the appropriate component (PH is Player)
                        if (go.GetComponent<CharacterData>() != null)//Player.cs is placeholder
                        {
                            savingObject.myCharacterData = new CharacterDataStruct();
                            savingObject.myCharacterData.myName = go.GetComponent<CharacterData>().myName;
                            savingObject.myCharacterData.myHealth = go.GetComponent<CharacterData>().myHealth;
                            savingObject.myCharacterData.myDamage = go.GetComponent<CharacterData>().myDamage;
                            savingObject.myCharacterData.myAttackSpeed = go.GetComponent<CharacterData>().myAttackSpeed;
                            savingObject.myCharacterData.myMoveSpeed = go.GetComponent<CharacterData>().myMoveSpeed;
                            savingObject.myCharacterData.myCastSpeed = go.GetComponent<CharacterData>().myCastSpeed;
                        }
                    }
                    else // it is not a prefab: bare-FBX, Light or Camera, e.g.
                    {
                        savingObject.myPath = AssetDatabase.GetAssetPath(PrefabUtility.GetCorrespondingObjectFromSource(go));
                    }

                    if (go.GetComponent<Collider>())
                    {
                        // Int or String? Int -> Enum Clas in C++ faster comparison. String might be fine during load.
                        // UnityEngine.collider, UnityEngine. ends on index 12 
                        savingObject.myColliderData = new ColliderData();
                        savingObject.myColliderData.myColliderType = go.GetComponent<Collider>().GetType().ToString().Remove(0, 12);
                        print(savingObject.myColliderData.myColliderType);
                        if (savingObject.myColliderData.myColliderType == "BoxCollider")
                        {
                            print("Getting BoxCollider Data");
                            savingObject.myColliderData.mySizeX = go.GetComponent<BoxCollider>().size.x;
                            savingObject.myColliderData.mySizeY = go.GetComponent<BoxCollider>().size.y;
                            savingObject.myColliderData.mySizeZ = go.GetComponent<BoxCollider>().size.z;
                            savingObject.myColliderData.myCenterX = go.GetComponent<BoxCollider>().center.x;
                            savingObject.myColliderData.myCenterY = go.GetComponent<BoxCollider>().center.y;
                            savingObject.myColliderData.myCenterZ = go.GetComponent<BoxCollider>().center.z;

                        }
                        else if (savingObject.myColliderData.myColliderType == "SphereCollider")
                        {
                            print("Getting SphereCollider Data");
                            savingObject.myColliderData.myRadius = go.GetComponent<SphereCollider>().radius;
                            savingObject.myColliderData.myCenterX = go.GetComponent<SphereCollider>().center.x;
                            savingObject.myColliderData.myCenterY = go.GetComponent<SphereCollider>().center.y;
                            savingObject.myColliderData.myCenterZ = go.GetComponent<SphereCollider>().center.z;
                        }
                    }

                    objects.dataList.Add(savingObject);
                }
            }
            string jsonString = JsonUtility.ToJson(objects, true);
            file.WriteLine(jsonString);
            
            print(System.DateTime.Now.ToString() + " - Level exported.");
        }
    }

    // [System.Serializable] is necessary. 
    // Without it before whatever class you are creating will not be included in the exported data file.
    [System.Serializable] 
    public class ColliderData
    {
        public string myColliderType;

        public float myCenterX;// World space?
        public float myCenterY;// World space?
        public float myCenterZ;// World space?
        //Sphere
        public float myRadius;
        //Box
        public float mySizeX;
        public float mySizeY;
        public float mySizeZ;
    }
    //public class BoxColliderData : ColliderData {}
    //public class SphereColliderData : ColliderData{}
    [System.Serializable]
    public class CharacterDataStruct
    {
        //These are all Place Holder [PH], examples
        public string myName;
        public float myHealth;
        public float myDamage;
        public float myMoveSpeed;
        public float myAttackSpeed;
        public float myCastSpeed;
    }
    [System.Serializable]
    public class Light
    {
        public string myType;
        public float myR;
        public float myG;
        public float myB;
        public float myA;
        public float myIntensity;
    }
    // Holds desired data. Unity Importer C++ must have the same values in its read. ( Found in .h file )
    [System.Serializable]
    public class ObjectData
    {
        public string myName;
        public float myRotX;// World space
        public float myRotY;// World space
        public float myRotZ;// World space
        public float myPosX;// World space
        public float myPosY;// World space
        public float myPosZ;// World space
        public float myScaleX;// World space
        public float myScaleY;// World space
        public float myScaleZ;// World space
        public string myPath;
        public CharacterDataStruct myCharacterData;
        public ColliderData myColliderData;
        /* Next step is to add several colliders. 
         * Being able to create a collider hierarchy for a character. 
         * So that LD can tweak hitboxes for weapons or areas of the body.
         * e.g:
         *  Player character - BoxCollider
         *      Player character ability aoe slam (example, not actual) - SphereCollider, placed in front of character
         *      Player character ability aoe slam (example, not actual) - SphereCollider
         *      Player character sword (swing) - BoxCollider.
         *      
        */
     }
    [System.Serializable]
    public struct ListContainer
    {
        public List<ObjectData> dataList;
        public ListContainer(List<ObjectData> aDataList)
        {
            dataList = aDataList;
        }
    }
}
// Character data export test backup
/*
 * /// find prefabs
        /// save data of prefabs to separate files
        /// GameObject Data
        using (System.IO.StreamWriter file = new System.IO.StreamWriter(target_path + "GameObjectData.txt"))
        {
            // Gets all root objects in the scene (i.e children are not included)
            GameObject[] rootObjects            = EditorSceneManager.GetActiveScene().GetRootGameObjects();
            List<string> addedObjects           = new List<string>();

            CharacterListContainer objects  = new CharacterListContainer();
            objects.dataList                = new List<CharacterData>();
            foreach (GameObject go in rootObjects)
            {
                var a = PrefabUtility.GetPrefabAssetType(go);
                if (a == PrefabAssetType.Regular && !addedObjects.Contains(go.GetComponent<Player>().myName))
                {
                    CharacterData data = new CharacterData();
                    data.myName         = go.GetComponent<Player>().myName;
                    data.myHealth       = go.GetComponent<Player>().myHealth;
                    data.myDamage       = go.GetComponent<Player>().myDamage;
                    data.myAttackSpeed  = go.GetComponent<Player>().myAttackSpeed;
                    data.myMoveSpeed    = go.GetComponent<Player>().myMoveSpeed;
                    data.myCastSpeed    = go.GetComponent<Player>().myCastSpeed;

                    addedObjects.Add(go.GetComponent<Player>().myName);
                    objects.dataList.Add(data);
                }
            }
        
            string jsonString = JsonUtility.ToJson(objects, true);
            file.WriteLine(jsonString);
        }

    [System.Serializable]
    public class CharacterData
    {
        public string myName;
        public float myHealth;
        public float myDamage;
        public float myMoveSpeed;
        public float myAttackSpeed;
        public float myCastSpeed;
    }
    [System.Serializable]
    public struct CharacterListContainer
    {
        public List<CharacterData> dataList;
        public CharacterListContainer(List<CharacterData> aDataList)
        {
            dataList = aDataList;
        }
    }
 */