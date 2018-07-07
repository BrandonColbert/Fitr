using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public abstract class InfinityStone : MonoBehaviour {
    [HideInInspector]
    public InfinityGauntlet gauntlet;
    public bool inUse;

    private Mesh selfMesh;
    private Material selfMat;

    public virtual void Start() {
        MeshFilter filter = GetComponent<MeshFilter>();
        selfMesh = Instantiate(filter.mesh);
        Vector3[] vertices = new Vector3[selfMesh.vertices.Length];
        for(int i = 0; i < selfMesh.vertices.Length; i++) {
            Vector3 v = selfMesh.vertices[i] * transform.localScale.x * 1.1f;
            vertices[i] = new Vector3(v.x, v.y, v.z);
        }
        selfMesh.vertices = vertices;
        selfMesh.RecalculateBounds();
        selfMesh.RecalculateNormals();

        MeshRenderer renderer = GetComponent<MeshRenderer>();
        selfMat = new Material(renderer.material);
        selfMat.shader = Shader.Find("Transparent/Diffuse");
        selfMat.color = new Color(selfMat.color.r, selfMat.color.g, selfMat.color.b, 0.5f);
    }

    public void Toggle() {
        StopCoroutine("Flash");
        StartCoroutine("Flash");
        OnActivate();
        OnDeactivate();
    }

    public void Toggle(bool state) {
        if(state) {
            OnActivate();
        } else {
            OnDeactivate();
        }

        inUse = state;
    }

    void Update() {
        if(inUse) {
            DisplayEffect();
        }
    }

    protected virtual void FixedUpdate () {
		if(inUse) {
            DuringUse();
        }
	}

    protected abstract void OnActivate();
    protected abstract void OnDeactivate();
    protected abstract void DuringUse();

    private IEnumerator Flash() {
        for(int i = 0; i < 20; i++) {
            DisplayEffect();
            yield return null;
        }
    }

    private void DisplayEffect() {
        Graphics.DrawMesh(selfMesh, transform.position, transform.rotation, selfMat, 0);
    }
}