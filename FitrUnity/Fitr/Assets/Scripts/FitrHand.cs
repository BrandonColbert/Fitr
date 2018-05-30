using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FitrHand : MonoBehaviour {
	[System.Serializable]
	public struct Finger {
		public bool connected;
		public GameObject a, b, c;
	}

	public GameObject palm;
	public Finger thumb, index, middle, ring, pinky;

	void Update() {
		foreach(Finger finger in GetFingers()) {
			if(finger.a != null) finger.a.SetActive(finger.connected);
			if(finger.b != null) finger.b.SetActive(finger.connected);
			if(finger.c != null) finger.c.SetActive(finger.connected);
		}
	}

	public void MoveFinger(Finger finger, Quaternion rotation, float flex) {
		if(finger.connected) {
			if(finger.c != null) {
				Vector3 flexAngle = Vector3.back * flex;
				finger.a.transform.localRotation = rotation;
				finger.b.transform.localRotation = Quaternion.Euler(flexAngle);
				finger.c.transform.localRotation = Quaternion.Euler(flexAngle * 1.2f);
			} else {
				finger.a.transform.localRotation = rotation;
				finger.b.transform.localRotation = Quaternion.Euler(Vector3.left * flex);
			}
		}
	}

	public Finger[] GetFingers() {
		return new Finger[] {
			thumb, index, middle, ring, pinky
		};
	}
}