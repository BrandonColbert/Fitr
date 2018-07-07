using UnityEngine;

public class StoneSpace : InfinityStone {
    public float teleportDistance = 250f;

    private bool didHit;
    private RaycastHit hit;
    private Mesh mesh;
    private Material mat;

    public override void Start() {
        base.Start();
        mesh = GetComponent<MeshFilter>().mesh;
        mat = GetComponent<MeshRenderer>().material;
    }

    protected override void DuringUse() {
        if(didHit = Physics.Raycast(transform.position, gauntlet.head.transform.forward, out hit, teleportDistance)) {
            Graphics.DrawMesh(mesh, hit.point, Quaternion.identity, mat, 0);
        }
    }

    protected override void OnActivate() {
        didHit = false;
    }

    protected override void OnDeactivate() {
        if(didHit) {
            Transform p = transform;
            while(p.parent) p = p.parent;

            p.transform.position = hit.point + Vector3.up * 2f;
        }
    }
}