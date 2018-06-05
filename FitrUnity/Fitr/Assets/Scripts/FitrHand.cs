using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FitrHand : MonoBehaviour {
	[System.Serializable]
	public struct Finger {
		public bool connected;
		public float flex;
		[System.NonSerialized]
		public float displayedFlex, smoothVelocity;
		public Quaternion rotation;
		public GameObject a, b, c;
	}

	public float smoothTime = 0.3f;
	public Quaternion palmRotation;
	public GameObject palm;
	public Finger thumb, index, middle, ring, pinky;

	private FitrMemory fitrMem;

	void Start() {
		fitrMem = new FitrMemory("FitrSharedMemory", 5);
	}

	void Update() {
		fitrMem.Sync(this);

		palm.transform.rotation = palmRotation;
		MoveFinger(ref thumb);
		MoveFinger(ref index);
		MoveFinger(ref middle);
		MoveFinger(ref ring);
		MoveFinger(ref pinky);
	}

	public void MoveFinger(ref Finger finger) {
		finger.displayedFlex = Mathf.SmoothDamp(finger.displayedFlex, finger.flex, ref finger.smoothVelocity, smoothTime);
		MoveFinger(ref finger, finger.rotation, finger.displayedFlex);
	}

	public void MoveFinger(ref Finger finger, Quaternion rotation, float flex) {
		if(finger.a != null) finger.a.SetActive(finger.connected);
		if(finger.b != null) finger.b.SetActive(finger.connected);
		if(finger.c != null) finger.c.SetActive(finger.connected);

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
}