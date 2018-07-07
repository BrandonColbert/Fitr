using UnityEngine;

public class StonePower : InfinityStone {
    public float punchRange = 50f;
    public float punchRadius = 10f;
    public float punchForce = 1000f;

    protected override void DuringUse() {}

    protected override void OnActivate() {
        RaycastHit hit;

        if(Physics.Raycast(gauntlet.head.transform.position, gauntlet.head.transform.forward, out hit, punchRange)) {
            Collider[] colliders = Physics.OverlapSphere(hit.point, punchRadius);

            foreach(Collider c in colliders) {
                Rigidbody rb = c.GetComponent<Rigidbody>();
                if(!rb && !c.GetComponent<FixedSpatialObject>()) rb = c.gameObject.AddComponent<Rigidbody>();

                if(rb) {
                    rb.AddExplosionForce(punchForce, hit.point, punchRadius);
                }
            }
        }
    }

    protected override void OnDeactivate() {}
}