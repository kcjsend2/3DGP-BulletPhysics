using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using System.IO;
using System.Text;

public class BinaryMeshWriter : MonoBehaviour
{
    void Write2DVector(BinaryWriter pStreamWriter, Vector2 v)
    {
        pStreamWriter.Write(v.x);
        pStreamWriter.Write(v.y);
    }

    void Write2DVectors(BinaryWriter pStreamWriter, string strName, Vector2[] pVectors)
    {
        pStreamWriter.Write(strName);
        pStreamWriter.Write(pVectors.Length);
        foreach (Vector2 v in pVectors) Write2DVector(pStreamWriter, v);
    }

    void Write3DVector(BinaryWriter pStreamWriter, Vector3 v)
    {
        pStreamWriter.Write(v.x);
        pStreamWriter.Write(v.y);
        pStreamWriter.Write(v.z);
    }

    void Write3DVectors(BinaryWriter pStreamWriter, string strName, Vector3[] pVectors)
    {
        pStreamWriter.Write(strName);
        pStreamWriter.Write(pVectors.Length);
        foreach (Vector3 v in pVectors) Write3DVector(pStreamWriter, v);
    }

    void WriteIntegers(BinaryWriter pStreamWriter, int[] pIntegers)
    {
        pStreamWriter.Write(pIntegers.Length);
        foreach (int i in pIntegers) pStreamWriter.Write(i);
    }

    void WriteIntegers(BinaryWriter pStreamWriter, string strName, int[] pIntegers)
    {
        pStreamWriter.Write(strName);
        pStreamWriter.Write(pIntegers.Length);
        foreach (int i in pIntegers) pStreamWriter.Write(i);
    }

    void WriteBoundingBox(BinaryWriter pStreamWriter, string strName, Bounds bounds)
    {
        pStreamWriter.Write(strName);
        Write3DVector(pStreamWriter, bounds.center);
        Write3DVector(pStreamWriter, bounds.extents);
    }

    void WriteSubMeshes(BinaryWriter pStreamWriter, string strName, Mesh mesh)
    {
        pStreamWriter.Write(strName);
        int nSubMeshes = mesh.subMeshCount;
        pStreamWriter.Write(nSubMeshes);
        for (int i = 0; i < nSubMeshes; i++)
        {
            uint nSubMeshStart = mesh.GetIndexStart(i);
            uint nSubMeshIndices = mesh.GetIndexCount(i);
            int[] pSubMeshIndices = mesh.GetIndices(i);
            pStreamWriter.Write(nSubMeshStart);
            pStreamWriter.Write(nSubMeshIndices);
            WriteIntegers(pStreamWriter, pSubMeshIndices);
        }
    }

    void WriteMesh(BinaryWriter pStreamWriter, Mesh mesh)
    {
        WriteBoundingBox(pStreamWriter, "<BoundingBox>:", mesh.bounds); //AABB

        Vector3[] pVertices = mesh.vertices;
        Vector3[] pNormals = mesh.normals;
        Vector2[] pUV1s = mesh.uv;

        if ((pVertices != null) && (pVertices.Length > 0)) Write3DVectors(pStreamWriter, "<Vertices>:", pVertices);
        if ((pNormals != null) && (pNormals.Length > 0)) Write3DVectors(pStreamWriter, "<Normals>:", pNormals);
        if ((pUV1s != null) && (pUV1s.Length > 0)) Write2DVectors(pStreamWriter, "<TextureCoords>:", pUV1s);

        int[] pIndices = mesh.triangles;
        if ((pIndices != null) && (pIndices.Length > 0)) WriteIntegers(pStreamWriter, "<Indices>:", pIndices);

        WriteSubMeshes(pStreamWriter, "<SubMeshes>:", mesh);
    }

    void Start()
    {
        BinaryWriter pStreamWriter = new BinaryWriter(File.Open(string.Copy(transform.name).Replace(" ", "_") + ".bin", FileMode.Create));

        MeshFilter meshFilter = transform.gameObject.GetComponent<MeshFilter>();
        MeshRenderer meshRenderer = transform.gameObject.GetComponent<MeshRenderer>();

        if (meshFilter && meshRenderer)
        {
            if (meshFilter.sharedMesh) WriteMesh(pStreamWriter, meshFilter.sharedMesh);
        }

        pStreamWriter.Flush();
        pStreamWriter.Close();

        print("Mesh Write Completed");
    }
}
