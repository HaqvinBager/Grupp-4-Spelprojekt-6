using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEngine;

public class JsonFinder
{
    private static string myTargetPath = "..\\IronWrought\\Bin\\json\\";

    public static void RefreshData(List<VFXData> anArray, string aPrefix)
    {
        List<string> jsonPaths = GetJsonPaths(aPrefix);

        if(jsonPaths.Count != anArray.Count)
        {
            if(jsonPaths.Count < anArray.Count)
            {
                anArray.RemoveRange(jsonPaths.Count, anArray.Count - jsonPaths.Count);
            }

            var temp = new VFXData[jsonPaths.Count];
            for(int i = 0; i < jsonPaths.Count; ++i)
            {
                temp[i] = new VFXData { myJson = jsonPaths[i], useMe = i < anArray.Count && anArray[i].useMe };
            }

            int count = 0;
            foreach(var data in temp)
            {
                if(count < anArray.Count)
                {
                    anArray[count] = new VFXData { myJson = temp[count].myJson, useMe = temp[count].useMe };
                }
                else
                {
                    anArray.Add(new VFXData { myJson = temp[count].myJson, useMe = false });
                }
                count++;
            }
        }

    }

    public static List<string> GetJsonPaths(string aPrefix)
    {
        string[] paths = Directory.GetFiles(myTargetPath, "*.json");
        List<string> particleJsonPaths = new List<string>();

        foreach (var path in paths)
        {
            int startIndex = path.IndexOf(aPrefix);
            if (startIndex > 0)
            {
                string particleJsonName = path.Substring(startIndex, (path.Length - startIndex) - 5);
                if (!particleJsonPaths.Contains(particleJsonName))
                {
                    particleJsonPaths.Add(particleJsonName);
                }
            }
        }

        return particleJsonPaths;
    }


}
