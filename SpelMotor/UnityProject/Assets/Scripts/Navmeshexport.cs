using System.Collections.Generic;
using System.IO;
using System.Text;
using UnityEditor;
using UnityEngine;
 
// Obj exporter component based on: http://wiki.unity3d.com/index.php?title=ObjExporter
 


public class ExportNavMeshToObj {
 
    static readonly string fileName = "..\\IronWrought\\Bin\\Levels\\";

    [MenuItem("Custom/[Deprecated] Export NavMesh to mesh")]
    public static void Export() {
        UnityEngine.AI.NavMeshTriangulation triangulatedNavMesh = UnityEngine.AI.NavMesh.CalculateTriangulation();
 
        Mesh mesh = new Mesh();
        mesh.name = "ExportedNavMesh";
        mesh.vertices = triangulatedNavMesh.vertices;
        mesh.triangles = triangulatedNavMesh.indices;
        string filename = fileName + Path.GetFileNameWithoutExtension(EditorApplication.currentScene) + "_ExportedNavMesh.obj";
        //"..\\IronWrought\\Bin\\Levels\\NavMesh\\" + Path.GetFileNameWithoutExtension(EditorApplication.currentScene) + "_ExportedNavMesh.obj";
        MeshToFile(mesh, filename);
        Debug.Log("NavMesh exported as '" + filename + "'");
        AssetDatabase.Refresh();
    }

    static string MeshToString(Mesh mesh) {
        StringBuilder sb = new StringBuilder();
 
        sb.Append("g ").Append(mesh.name).Append("\n");
        foreach (Vector3 v in mesh.vertices) {
            sb.Append(string.Format("v {0} {1} {2}\n",v.x,v.y,v.z));
        }
        sb.Append("\n");
        foreach (Vector3 v in mesh.normals) {
            sb.Append(string.Format("vn {0} {1} {2}\n",v.x,v.y,v.z));
        }
        sb.Append("\n");
        foreach (Vector3 v in mesh.uv) {
            sb.Append(string.Format("vt {0} {1}\n",v.x,v.y));
        }
        for (int material = 0; material < mesh.subMeshCount; material++) {
            sb.Append("\n");
            //sb.Append("usemtl ").Append(mats[material].name).Append("\n");
            //sb.Append("usemap ").Append(mats[material].name).Append("\n");
 
            int[] triangles = mesh.GetTriangles(material);
            for (int i=0;i<triangles.Length;i+=3) {
                sb.Append(string.Format("f {0}/{0}/{0} {1}/{1}/{1} {2}/{2}/{2}\n", triangles[i]+1, triangles[i+1]+1, triangles[i+2]+1));
            }
        }
        return sb.ToString();
    }
 
    static void MeshToFile(Mesh mesh, string filename) {
        using (StreamWriter sw = new StreamWriter(filename)) {
            sw.Write(MeshToString(mesh));
        }
    }
}

//public struct NavMeshData
//{
//    public Vector3[] myVerticies;
//    public Vector3[] myNormals;
//    public Vector2[] myUVs;
//    public int[] myTriangles;
//}

//static NavMeshData MeshToData(Mesh mesh)
//{
//    NavMeshData navMeshData = new NavMeshData();

//    mesh.vertices.CopyTo(navMeshData.myVerticies, 0);
//    mesh.normals.CopyTo(navMeshData.myNormals, 0);
//    mesh.uv.CopyTo(navMeshData.myUVs, 0);

//    List<int> triangles = new List<int>();
//    for (int material = 0; material < mesh.subMeshCount; material++)
//    {
//        triangles.AddRange(mesh.GetTriangles(material));       
//    }

//    navMeshData.myTriangles = triangles.ToArray();

//    return navMeshData;
//}