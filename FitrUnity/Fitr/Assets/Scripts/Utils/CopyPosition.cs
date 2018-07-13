using UnityEngine;

public class CopyPosition : MonoBehaviour {
    public Transform source;

    void FixedUpdate() {
        if(source) transform.position = source.position;
    }

    void OnDrawGizmosSelected() {
        if(source) {
            Gizmos.color = Color.blue;
            Gizmos.DrawCube(source.position, transform.localScale * 0.1f);
        }
    }
}