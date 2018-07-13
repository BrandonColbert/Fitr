using UnityEngine;

public class Handedness : MonoBehaviour {
    public FitrHand hand;

    void Start() {
        Vector3 v = transform.localPosition;
		v.x *= hand.handedness == FitrHand.HandType.LEFT ? 1 : -1;
		transform.localPosition = v;
    }
}