using UnityEngine;

public class StoneReality : InfinityStone {
    public float alterRange = 40f;
    public GameObject[] transformationPrefabs;

    protected override void DuringUse() {

    }

    protected override void OnActivate() {
        RaycastHit hit;

        if(Physics.Raycast(gauntlet.head.transform.position, gauntlet.head.transform.forward, out hit, alterRange)) {
            if(!hit.transform.GetComponent<FixedSpatialObject>()) {
                GameObject tr = transformationPrefabs[Random.Range(0, transformationPrefabs.Length)];

                MeshFilter filter = hit.transform.GetComponent<MeshFilter>();
                if(filter) filter.mesh = tr.GetComponent<MeshFilter>().sharedMesh;

                MeshRenderer renderer = hit.transform.GetComponent<MeshRenderer>();
                if(renderer) renderer.materials = tr.GetComponent<MeshRenderer>().sharedMaterials;

                hit.transform.localScale = tr.transform.localScale;

                if(!hit.transform.GetComponent<Rigidbody>()) hit.transform.gameObject.AddComponent<Rigidbody>();
            }
        }
    }

    protected override void OnDeactivate() {

        
    }
}