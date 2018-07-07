using UnityEngine;

public class StoneMind : InfinityStone {
    public float psychicRange = 250f;
    public float highlightScale = 1.2f;

    private GameObject hitObj;
    private RaycastHit hit;
    private float distance;
    private Rigidbody rb;

    private Vector3 velocity, lastPosition;
    private Mesh mesh;
    private Material mat;

    protected override void DuringUse() {
        if(hitObj) {
            if(rb) {
                rb.velocity = Vector3.zero;
                velocity = (hitObj.transform.position - lastPosition) / Time.deltaTime * 0.1f;
                lastPosition = hitObj.transform.position;
            }

            if(Physics.Raycast(gauntlet.head.transform.position, gauntlet.head.transform.forward, out hit, distance * 2f)) {
                if(hit.transform.gameObject != hitObj) {
                    hitObj.transform.position = hit.point + Vector3.up;
                    distance = Vector3.Distance(hitObj.transform.position, gameObject.transform.position);
                }
            } else {
                hitObj.transform.position = gauntlet.head.transform.position + gauntlet.head.transform.forward * distance;
            }

            Graphics.DrawMesh(mesh, hitObj.transform.position, hitObj.transform.rotation, mat, 0);
        }
    }

    protected override void OnActivate() {
        if(Physics.Raycast(gauntlet.head.transform.position, gauntlet.head.transform.forward, out hit, psychicRange)) {
            if(!hit.transform.gameObject.GetComponent<FixedSpatialObject>()) {
                hitObj = hit.transform.gameObject;
                distance = Vector3.Distance(hitObj.transform.position, gameObject.transform.position);     
        
                rb = hitObj.GetComponent<Rigidbody>();
                if(!rb) rb = hitObj.AddComponent<Rigidbody>();

                velocity = Vector3.zero;
                lastPosition = hitObj.transform.position;

                MeshFilter filter = hitObj.GetComponent<MeshFilter>();
                mesh = Instantiate(filter.mesh);
                Vector3[] vertices = new Vector3[mesh.vertices.Length];
                for(int i = 0; i < mesh.vertices.Length; i++) {
                    Vector3 v = mesh.vertices[i] * highlightScale;
                    vertices[i] = new Vector3(v.x * hitObj.transform.localScale.x, v.y * hitObj.transform.localScale.y, v.z * hitObj.transform.localScale.z);
                }
                mesh.vertices = vertices;
                mesh.RecalculateBounds();
                mesh.RecalculateNormals();

                MeshRenderer renderer = hitObj.GetComponent<MeshRenderer>();
                mat = new Material(renderer.material);
                mat.shader = Shader.Find("Transparent/Diffuse");
                mat.color = new Color(mat.color.r, mat.color.g, mat.color.b, 0.5f);
            }
        }
    }

    protected override void OnDeactivate() {
        if(rb) rb.velocity = velocity;
        hitObj = null;
    }
}