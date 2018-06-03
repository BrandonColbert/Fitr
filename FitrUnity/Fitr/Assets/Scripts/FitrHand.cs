using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FitrHand : MonoBehaviour {
	[System.Serializable]
	public struct Finger {
		public bool connected;
		public float flex;
		public float velocity;
		public Quaternion rotation;
		public GameObject a, b, c;
	}

	public GameObject palm;
	public Finger thumb, index, middle, ring, pinky;

	public float smoothTime = 0.3f;

	void Start() {
		if(CPPInterface.IsConnected()) {
			CPPInterface.Disconnect();
		}

		CPPInterface.Connect();
	}

	void OnApplicationQuit() {
        if(CPPInterface.IsConnected()) {
			CPPInterface.Disconnect();
		}
    }

	void Update() {
		if(CPPInterface.IsConnected()) {
			IntPtr hand = CPPInterface.GetHand();
			IntPtr fingerIndex = CPPInterface.GetFinger(hand, 1);
			float indexFlex = CPPInterface.GetFingerFlex(fingerIndex);
			index.flex = Mathf.SmoothDamp(index.flex, indexFlex, ref index.velocity, smoothTime);
			index.rotation = Quaternion.identity;
			MoveFinger(index);

			Debug.Log("Flex of Finger 1: " + indexFlex);
		} else {
			Debug.Log("Not connected");
		}

		foreach(Finger finger in GetFingers()) {
			if(finger.a != null) finger.a.SetActive(finger.connected);
			if(finger.b != null) finger.b.SetActive(finger.connected);
			if(finger.c != null) finger.c.SetActive(finger.connected);
		}
	}

	public void MoveFinger(Finger finger) {
		MoveFinger(finger, finger.rotation, finger.flex);
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