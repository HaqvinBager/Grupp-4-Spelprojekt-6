using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using UnityEditor.SceneManagement;
using System.Text;

public class PrefabCreator
{
    private static readonly string[] prefabFolders =
    {
        "EN",
        "CH",
        "VF",
        "UI"
    };

    [MenuItem("Tools/Create Prefabs")]
    public static void CreatePrefabs()
    {
        SceneSetup[] setup = EditorSceneManager.GetSceneManagerSetup();

        GetAllSourceAssetNamesAndGameObjects(out List<string> sourceAssetNames, out List<GameObject> uniqueAssetsToMakeIntoPrefabs);
        PrintAssetNames(sourceAssetNames);

        List<GameObject> createdPrefabs = new List<GameObject>();
        int index = 0;
        foreach (GameObject sourceAsset in uniqueAssetsToMakeIntoPrefabs)
        {
            GameObject newPrefab = new GameObject(sourceAssetNames[index]);
            Undo.RecordObject(newPrefab, "Created Temporary GameObject");

            GameObject newChild = GameObject.Instantiate(sourceAsset, newPrefab.transform);
            newChild.transform.position = Vector3.zero;
            newChild.transform.rotation = Quaternion.identity;
            newChild.transform.localScale = new Vector3(1, 1, 1);
            Undo.RecordObject(newChild, "Created a Copy of the Source Source Asset");

            GameObject createdPrefab = PrefabUtility.SaveAsPrefabAssetAndConnect(newPrefab, GetCreatePrefabPath(sourceAsset.name), InteractionMode.UserAction);
            createdPrefabs.Add(createdPrefab);
            Undo.DestroyObjectImmediate(newPrefab);

            index++;
        }

        AssetDatabase.Refresh();


        if (!EditorSceneManager.SaveCurrentModifiedScenesIfUserWantsTo())
        {
            EditorSceneManager.RestoreSceneManagerSetup(setup);
        }

    }

    private static bool IsValidPrefab(string assetName)
    {
        //string prefabPath = "Assets/Prefabs/";
        foreach (string prefabFolder in prefabFolders)
        {
            if (assetName.Substring(0, 2).Equals(prefabFolder))
            {
                return true;
            }
        }
        //Debug.LogError("Failed to Create Prefab Path");
        return false;
    }

    private static string GetCreatePrefabPath(string assetName)
    {
        string prefabPath = "Assets/Prefabs/";
        foreach (string prefabFolder in prefabFolders)
        {
            if (assetName.Substring(0, 2).Equals(prefabFolder))
            {
                prefabPath += prefabFolder + "/";
                if (!AssetDatabase.IsValidFolder("Assets/Prefabs/" + prefabFolder))
                {
                    AssetDatabase.CreateFolder("Assets/Prefabs", prefabFolder);
                }
                prefabPath += assetName + "_Prefab.prefab";
                return prefabPath;
            }
        }
        Debug.LogError("Failed to Create Prefab Path");
        return "Assets/NA";
    }

    private static void GetAllSourceAssetNamesAndGameObjects(out List<string> sourceAssetNames, out List<GameObject> uniqueAssetsToMakeIntoPrefabs)
    {
        sourceAssetNames = new List<string>();
        uniqueAssetsToMakeIntoPrefabs = new List<GameObject>();
        GameObject[] allGameObjects = GameObject.FindObjectsOfType<GameObject>();
        foreach (GameObject gameObject in allGameObjects)
        {
            if (!IsValidPrefab(gameObject.name))
                continue;

            PrefabAssetType type = PrefabUtility.GetPrefabAssetType(gameObject);
            if (type == PrefabAssetType.Model)
            {
                GameObject originalSource = PrefabUtility.GetCorrespondingObjectFromOriginalSource(gameObject);
                if (originalSource != null)
                {
                    if (gameObject.transform.childCount == 0)
                    {
                        string assetName = originalSource.name;
                        if (!sourceAssetNames.Contains(assetName))
                        {
                            sourceAssetNames.Add(assetName);
                            uniqueAssetsToMakeIntoPrefabs.Add(originalSource);
                            Debug.Log("Unique Asset: " + originalSource.name, originalSource);
                        }
                    }
                }
            }
        }
    }

    private static void PrintAssetNames(List<string> sourceAssetNames)
    {
        StringBuilder stringBuilder = new StringBuilder();
        foreach (string name in sourceAssetNames)
        {
            stringBuilder.Append(name + "\n");
        }
        Debug.Log(stringBuilder.ToString());
    }

    //private static void ChangeObjectIntoPrefab(GameObject gameObjectInstance, GameObject gameObjectSource)
    //{
    //    //if (AssetDatabase.Contains(gameObjectSource.GetInstanceID()))
    //    //{
    //    //    Debug.LogWarning("Prefab Folder Already Contains: [" + gameObjectSource.name + "] prefab!", gameObjectSource);
    //    //    return;
    //    //}

    //    string prefabPath = "Assets/Prefabs/";

    //    string gameObjectType = gameObjectInstance.name.Substring(0, 2);
    //    foreach (string prefabFolder in prefabFolders)
    //    {
    //        if (gameObjectType.Equals(prefabFolder))
    //        {
    //            prefabPath += prefabFolder + "/";
    //            if (!AssetDatabase.IsValidFolder("Assets/Prefabs/" + prefabFolder))
    //            {
    //                AssetDatabase.CreateFolder("Assets/Prefabs", prefabFolder);
    //            }
    //            break;
    //        }
    //    }

    //    prefabPath += gameObjectInstance.name + "_Prefab.prefab";

    //    GameObject parentObject = new GameObject();
    //    parentObject.name = gameObjectInstance.name + "_Prefab";
    //    parentObject.transform.position = gameObjectInstance.transform.position;
    //    gameObjectInstance.transform.SetParent(parentObject.transform);

    //    GameObject savedPrefab = PrefabUtility.SaveAsPrefabAssetAndConnect(parentObject, prefabPath, InteractionMode.UserAction);
    //    savedPrefab.transform.position = Vector3.zero;
    //    savedPrefab.transform.rotation = Quaternion.identity;
    //    savedPrefab.transform.localScale = new Vector3(1, 1, 1);
    //}
}



/* 3: Använd listan av Unika namn för att Skapa Prefabs Med originalSourcen som Child */






//GameObject[] selectedObjects = Selection.gameObjects;
//foreach (GameObject selectedObject in selectedObjects)
//{
//    GameObject parentOfObject = PrefabUtility.GetOutermostPrefabInstanceRoot(selectedObject);
//    GameObject objectPrefab = PrefabUtility.GetCorrespondingObjectFromOriginalSource(parentOfObject);
//    PrefabAssetType prefabAssetType = PrefabUtility.GetPrefabAssetType(objectPrefab);

//    if (objectPrefab == null)           
//        continue;

//    if (prefabAssetType == PrefabAssetType.Model)
//    {
//        ChangeObjectIntoPrefab(selectedObject, objectPrefab);
//    }
//}

//switch (gameObjectName.Substring(0, 2)) {
//    case "EN":
//        prefabPath += "EN/";
//        break;
//    case "CH":
//        prefabPath += "CH/";
//        break;
//    case "VF":
//        prefabPath += "VFX/";
//        break;
//    case "UI":
//        prefabPath += "UI/";
//        break;
//}


//GameObject[] allGameObjects = GameObject.FindObjectsOfType<GameObject>();
//foreach (GameObject gameObject in allGameObjects)
//{
//    string name = gameObject.name;
//    foreach (string prefabName in sourceAssetNames)
//    {
//        if (name.Contains(prefabName))
//        {
//            GameObject prefab = createdPrefabs.Find(e => e.name.Contains(name));
//            if(prefab != null)
//            {
//                GameObject objectThatReplacesOldObject = (GameObject)PrefabUtility.InstantiatePrefab(prefab);
//                Undo.RecordObject(objectThatReplacesOldObject, "Created Prefab");
//                objectThatReplacesOldObject.transform.position = gameObject.transform.position;
//            }
//        }
//    }
//}

//for (int i = allGameObjects.Length - 1; i >= 0; --i)
//{
//    if(allGameObjects[i] != null)
//        Undo.DestroyObjectImmediate(allGameObjects[i]);
//}