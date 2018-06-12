using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
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

	[Range(0, 23)]
	public int currentMutation = 0;

	public PlugValue xOut, yOut, zOut, wOut;

	public float smoothTime = 0.3f;
	public Quaternion palmRotation;
	public GameObject palm;
	public Finger thumb, index, middle, ring, pinky;

	private FitrMemory fitrMem;

	public PlugValue xIn, yIn, zIn, wIn;

	private void Init() {
		fitrMem = new FitrMemory("FitrSharedMemory", 5);
	}

	void Start() {
		Init();
	}

	IEnumerable<IEnumerable<T>> GetPermutations<T>(IEnumerable<T> list, int length) {
		if(length == 1) {
			return list.Select(t => new T[] { t });
		}

    	return GetPermutations(list, length - 1).SelectMany(t => list.Where(o => !t.Contains(o)), (t1, t2) => t1.Concat(new T[] { t2 }));
	}

	void Update() {
		if(fitrMem == null) Init();
		fitrMem.Sync(this);
/*
		xIn.value = palmRotation.x;
		yIn.value = palmRotation.y;
		zIn.value = palmRotation.z;
		wIn.value = palmRotation.w;

		int[] final = new int[4];

// x  y  z  w ->  a  b  c  d at i0
// w  x  z  y <-  d  a  c  b at i19 from last
//-y  z  x -w <- -d  c  b  -a at i23 from last
		var permutations = GetPermutations(new List<int>{'a', 'b', 'c', 'd'}, 4);

		if(currentMutation >= permutations.Count()) {
			currentMutation = permutations.Count() - 1;
		}

		string s = "";
		foreach(var c in permutations.ElementAt(currentMutation)) {
			s += (char)c + " ";
		}
		Debug.Log("Testing mutation: " + (currentMutation + 1) + "/" + permutations.Count() + " aka \"" + s + "\"");

		int zed = 0;
		foreach(char i in permutations.ElementAt(currentMutation)) {
			final[zed] = i - 'a';
			zed++;
		}

		xOut = final[0] == 0 ? xIn : final[0] == 1 ? yIn : final[0] == 2 ? zIn : final[0] == 3 ? wIn : null;
		yOut = final[1] == 0 ? xIn : final[1] == 1 ? yIn : final[1] == 2 ? zIn : final[1] == 3 ? wIn : null;
		zOut = final[2] == 0 ? xIn : final[2] == 1 ? yIn : final[2] == 2 ? zIn : final[2] == 3 ? wIn : null;
		wOut = final[3] == 0 ? xIn : final[3] == 1 ? yIn : final[3] == 2 ? zIn : final[3] == 3 ? wIn : null;

		palm.transform.localRotation = new Quaternion(-xOut.value, yOut.value, zOut.value, -wOut.value);
*/

		palm.transform.localRotation = palmRotation;
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
		/*
		if(finger.a != null) finger.a.SetActive(finger.connected);
		if(finger.b != null) finger.b.SetActive(finger.connected);
		if(finger.c != null) finger.c.SetActive(finger.connected);
		*/

		if(finger.connected) {
			Vector3 flexAngle = Vector3.right * flex;
			finger.a.transform.rotation = rotation;
			finger.b.transform.localRotation = Quaternion.Euler(flexAngle);
			if(finger.c != null) finger.c.transform.localRotation = Quaternion.Euler(flexAngle * 1.2f);
		} else {
			if(finger.a != null) finger.a.transform.localRotation = Quaternion.identity;
			if(finger.b != null) finger.b.transform.localRotation = Quaternion.identity;
			if(finger.c != null) finger.c.transform.localRotation = Quaternion.identity;
		}
	}
}