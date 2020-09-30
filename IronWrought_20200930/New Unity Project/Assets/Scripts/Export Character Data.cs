using UnityEngine;
using UnityEditor;
using System.Collections;
using System.Collections.Generic;
using UnityEditor.SceneManagement;
using UnityEngine.SceneManagement;


// 2020-09-29 OBSOLETE. Might be used later if it is decided that CharacterData should be separated from LevelData.
// Fetches prefabs in scene and saves their values( if changed in the editor these changes will be represented.) 
// Only fetches 1 per prefab. i.e
//      EnemyPrefab (even if there are 100 enemies in the scene, the first one that is read will define all entities of that type)
//          data
//      PlayerPrefab
//          data
//      BossPrefab
//          data

public class ExportCharacterData : MonoBehaviour
{
    //[MenuItem("Tools/Export Character Data #u")]
    //private static void NewMenuOption()
    //{
    //    string target_path = "..\\Bin\\Unity Level Export\\";
    //    if (!System.IO.Directory.Exists(target_path))
    //    {
    //        System.IO.Directory.CreateDirectory(target_path);
    //    }
    //
    //    //var prefabs = AssetDatabase.FindAssets("_prefab t:GameObject", new[] { "Assets/Prefabs" });
    //    //foreach(string s in prefabs)
    //    //{
    //    //    print(s);
    //    //}
    //    using (System.IO.StreamWriter file = new System.IO.StreamWriter(target_path + "CharacterData.json"))
    //    {
    //        // Gets all root objects in the scene (i.e children are not included)
    //        GameObject[] rootObjects = EditorSceneManager.GetActiveScene().GetRootGameObjects();
    //        List<string> addedObjects = new List<string>();
    //
    //        CharacterListContainer objects = new CharacterListContainer();
    //        objects.dataList = new List<CharacterData>();
    //        foreach (GameObject go in rootObjects)
    //        {
    //            var a = PrefabUtility.GetPrefabAssetType(go);
    //            if (a == PrefabAssetType.Regular && !addedObjects.Contains(go.GetComponent<Player>().myName))
    //            {
    //                CharacterData data = new CharacterData();
    //                data.myName = go.GetComponent<Player>().myName;
    //                data.myHealth = go.GetComponent<Player>().myHealth;
    //                data.myDamage = go.GetComponent<Player>().myDamage;
    //                data.myAttackSpeed = go.GetComponent<Player>().myAttackSpeed;
    //                data.myMoveSpeed = go.GetComponent<Player>().myMoveSpeed;
    //                data.myCastSpeed = go.GetComponent<Player>().myCastSpeed;
    //
    //                addedObjects.Add(go.GetComponent<Player>().myName);
    //                objects.dataList.Add(data);
    //            }
    //        }
    //
    //        string jsonString = JsonUtility.ToJson(objects, true);
    //        file.WriteLine(jsonString);
    //    }
    //}
    //
    //[System.Serializable]
    //public class CharacterData
    //{
    //    public string myName;
    //    public float myHealth;
    //    public float myDamage;
    //    public float myMoveSpeed;
    //    public float myAttackSpeed;
    //    public float myCastSpeed;
    //}
    //
    //[System.Serializable]
    //public struct CharacterListContainer
    //{
    //    public List<CharacterData> dataList;
    //    public CharacterListContainer(List<CharacterData> aDataList)
    //    {
    //        dataList = aDataList;
    //    }
    //}
}
