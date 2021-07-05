using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using System.IO;
using System.Text;

public class TextMeshWriter : MonoBehaviour
{
    void Write2DVectors(StreamWriter pStreamWriter, string strName, Vector2[] pVectors)
    {
        pStreamWriter.Write(strName);
        pStreamWriter.Write(" " + pVectors.Length);
        foreach (Vector2 v in pVectors) pStreamWriter.Write(" " + v.x + " " + v.y);
        pStreamWriter.WriteLine(" ");
    }

    void Write3DVectors(StreamWriter pStreamWriter, string strName, Vector3[] pVectors)
    {
        pStreamWriter.Write(strName);
        pStreamWriter.Write(" " + pVectors.Length);
        foreach (Vector3 v in pVectors) pStreamWriter.Write(" " + v.x + " " + v.y + " " + v.z); 
        pStreamWriter.WriteLine(" ");
    }

    void WriteIntegers(StreamWriter pStreamWriter, string strName, int[] pIntegers)
    {
        pStreamWriter.Write(strName);
        pStreamWriter.Write(" " + pIntegers.Length);
        foreach (int i in pIntegers) pStreamWriter.Write(" " + i);
        pStreamWriter.WriteLine(" ");
    }

    void WriteMesh(StreamWriter pStreamWriter, Mesh mesh)
    {
        Vector3[] pVertices = mesh.vertices;
        Vector3[] pNormals = mesh.normals;
        Vector2[] pUV1s = mesh.uv;

        if ((pVertices != null) && (pVertices.Length > 0)) Write3DVectors(pStreamWriter, "<Vertices>:", pVertices);
        if ((pNormals != null) && (pNormals.Length > 0)) Write3DVectors(pStreamWriter, "<Normals>:", pNormals);
        if ((pUV1s != null) && (pUV1s.Length > 0)) Write2DVectors(pStreamWriter, "<TextureCoords>:", pUV1s);

        int[] pIndices = mesh.triangles;
        if ((pIndices != null) && (pIndices.Length > 0)) WriteIntegers(pStreamWriter, "<Indices>:", pIndices);
    }

    void Start()
    {
        StreamWriter pStreamWriter = new StreamWriter(File.Open(string.Copy(transform.name).Replace(" ", "_") + ".txt", FileMode.Create));

        MeshFilter meshFilter = GetComponent<MeshFilter>();
        MeshRenderer meshRenderer = GetComponent<MeshRenderer>();

        if (meshFilter && meshRenderer)
        {
            if (meshFilter.sharedMesh) WriteMesh(pStreamWriter, meshFilter.sharedMesh);
        }

        pStreamWriter.Flush();
        pStreamWriter.Close();

        print("Mesh Write Completed");
    }
}
