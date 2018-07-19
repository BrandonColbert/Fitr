using UnityEngine;

public class StoneSoul : InfinityStone {
    public float killRange = 100f;

    protected override void DuringUse() {}

    protected override void OnActivate() {
        RaycastHit hit;
        if(Physics.Raycast(gauntlet.head.transform.position, gauntlet.head.transform.forward, out hit, killRange)) {
            Living entity = hit.transform.gameObject.GetComponent<Living>();
            
            if(entity) {
                entity.Kill();
            }
        }
    }

    protected override void OnDeactivate() {}
}