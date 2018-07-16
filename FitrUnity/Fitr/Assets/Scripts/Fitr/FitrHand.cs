using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;

///<summary>Keep fingers straight/in the default position while starting Unity Aplication (not FitrWin.exe) (in other words, make sure the MPUs align perfectly)</summary>
public class FitrHand : MonoBehaviour {
	[System.Serializable]
	public struct Finger {
		public bool connected;
		public float flex;
		[System.NonSerialized]
		public float displayedFlex, smoothVelocity;
		public FitrQuaternion rotation;
		public GameObject a, b, c;
	}

	public enum HandType {
		RIGHT,
		LEFT
	}

	public bool originSet = false;
	public bool displayInactiveFingers = true;
	public bool showFlex = true;
	public HandType handedness;
	public float smoothTime = 0.3f;
	public GameObject palm;
	public FitrQuaternion palmRotation;
	public Finger thumb, index, middle, ring, pinky;
	private FitrMemory fitrMem;

	private void Init() {
		fitrMem = new FitrMemory("FitrSharedMemory", 5);
	}

	void Start() {
		Init();
	}

	void Update() {
		if(fitrMem == null) Init();
		fitrMem.Sync(this);

		if(palm) {
			palm.transform.localRotation = palmRotation.d;
		}

		MoveFinger(ref thumb);
		MoveFinger(ref index);
		MoveFinger(ref middle);
		MoveFinger(ref ring);
		MoveFinger(ref pinky);

		if(!originSet) {
			palmRotation.hasOrigin = false;
			thumb.rotation.hasOrigin = false;
			index.rotation.hasOrigin = false;
			middle.rotation.hasOrigin = false;
			ring.rotation.hasOrigin = false;
			pinky.rotation.hasOrigin = false;
			originSet = true;
		}
	}	

	public void MoveFinger(ref Finger finger) {
		finger.displayedFlex = Mathf.SmoothDamp(finger.displayedFlex, finger.flex, ref finger.smoothVelocity, smoothTime);
		MoveFinger(ref finger, finger.rotation, finger.displayedFlex);
	}

	public void MoveFinger(ref Finger finger, FitrQuaternion rotation, float flex) {
		if(!displayInactiveFingers) {
			if(finger.a != null) finger.a.SetActive(finger.connected);
			if(finger.b != null) finger.b.SetActive(finger.connected);
			if(finger.c != null) finger.c.SetActive(finger.connected);
		}

		if(finger.connected) {
			finger.rotation.o = palmRotation.o;
			finger.a.transform.localRotation = rotation.v;

			if(showFlex) {
				Vector3 flexAngle = Vector3.left * flex * 100f;
				finger.b.transform.localRotation = Quaternion.Euler(flexAngle);
				if(finger.c != null) finger.c.transform.localRotation = Quaternion.Euler(flexAngle * 1.2f);
			}
		} else {
			if(finger.a != null) {
				finger.a.transform.localRotation = palm.transform.localRotation;
			}
			if(finger.b != null) finger.b.transform.localRotation = Quaternion.identity;
			if(finger.c != null) finger.c.transform.localRotation = Quaternion.identity;
		}
	}

	public Finger[] GetFingers() {
		return new Finger[]{thumb, index, middle, ring, pinky};
	}
}