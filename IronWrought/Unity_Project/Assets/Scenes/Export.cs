using UnityEngine;
using UnityEditor;
using System.Collections;
using System.Collections.Generic;
using UnityEditor.SceneManagement;
using UnityEngine.SceneManagement;

public class MenuItems
{
    // REMINDER: add changes made here to Binary export script.

    [MenuItem("Tools/Export all #s")]
    private static void NewMenuOption()
    {
        string target_path = "..\\Bin\\Unity Level Export\\";
        if (!System.IO.Directory.Exists(target_path))
        {
            System.IO.Directory.CreateDirectory(target_path);
        }

        string scene_name = SceneManager.GetActiveScene().name;

        using (System.IO.StreamWriter file = new System.IO.StreamWriter(target_path + scene_name + "_exportedLevelASCII.txt"))
        {
            GameObject[] rootObjects = EditorSceneManager.GetActiveScene().GetRootGameObjects();

            // Gets all root objects in the scene (i.e children are not included)
            List<GameObject> gameObjectsInScene = new List<GameObject>();
            foreach(GameObject go in rootObjects)
            {
                gameObjectsInScene.Add(go);
            }

            // Future additions/ changes
            //GameObject[] allObjects       = UnityEngine.Object.FindObjectsOfType<GameObject>();// FBX Transforms
            //Light[] allLights             = UnityEngine.Object.FindObjectsOfType<Light>();
            //Collider[] allColliders       = UnityEngine.Object.FindObjectsOfType<Collider>();

            
            ListContainer objects = new ListContainer();
            objects.dataList = new List<ObjectData>();
            //List<ObjectData> savingObjects = new List<ObjectData>();
            foreach (GameObject go in gameObjectsInScene)
            {
                // Get desired data of root objects

                //if (go.CompareTag("Export"))// Change to check if root has child that is mesh. But grab 
                //{
                    ObjectData savingObject = new ObjectData();
                    savingObject.myRotX = go.transform.rotation.eulerAngles.x;//Degrees
                    savingObject.myRotY = go.transform.rotation.eulerAngles.y;//Degrees
                    savingObject.myRotZ = go.transform.rotation.eulerAngles.z;//Degrees
                    savingObject.myPosX = go.transform.position.x;
                    savingObject.myPosY = go.transform.position.y;
                    savingObject.myPosZ = go.transform.position.z;

                    savingObject.myScaleX = go.transform.localScale.x;
                    savingObject.myScaleY = go.transform.localScale.y;
                    savingObject.myScaleZ = go.transform.localScale.z;

                    savingObject.myName = go.name;
                    savingObject.myPath = AssetDatabase.GetAssetPath(PrefabUtility.GetCorrespondingObjectFromSource(go));

                    //if (go.GetComponent<Light>())
                    //{
                    //    savingObject.myIntensity    = go.GetComponent<Light>().intensity;
                    //    savingObject.myR            = go.GetComponent<Light>().color.r;
                    //    savingObject.myG            = go.GetComponent<Light>().color.g;
                    //    savingObject.myB            = go.GetComponent<Light>().color.b;
                    //    savingObject.myA            = go.GetComponent<Light>().color.a;
                    //    savingObject.myLightType    = go.GetComponent<Light>().type;
                    //}
                    //
                    //if (go.GetComponent<BoxCollider>())
                    //{
                    //    savingObject.myColliderSizeX = go.GetComponent<BoxCollider>().size.x;
                    //    savingObject.myColliderSizeY = go.GetComponent<BoxCollider>().size.y;
                    //    savingObject.myColliderSizeZ = go.GetComponent<BoxCollider>().size.z;
                    //}

                    objects.dataList.Add(savingObject);
               // }
                
            }
            string jsonString = JsonUtility.ToJson(objects, true);
            file.WriteLine(jsonString);
        }
    }

    [System.Serializable]
    public class ObjectData
    {
        public string myName;
        public float myRotX;
        public float myRotY;
        public float myRotZ;
        public float myPosX;
        public float myPosY;
        public float myPosZ;
        public float myScaleX;
        public float myScaleY;
        public float myScaleZ;
        public string myPath;

        //public float myIntensity;
        //public float myR;
        //public float myG;
        //public float myB;
        //public float myA;
        //public LightType myLightType;//Ger ett nummer, är en enum class
        //
        //public float myColliderSizeX;
        //public float myColliderSizeY;
        //public float myColliderSizeZ;

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
