using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FingerMotionTest : MonoBehaviour {
	[System.Serializable]
	public struct FingerMotion {
		public int fingerNumber;
		public Vector3 rotation;
		public float flex;
	}

	public FitrHand hand;
	public bool autoTest = false;

	public FingerMotion[] testers;

	void FixedUpdate() {
		for(int i = 0; i < testers.Length; i++) {
			FingerMotion fm = testers[i];

			FitrHand.Finger[] fingers = hand.GetFingers();
			if(0 <= fm.fingerNumber && fm.fingerNumber < fingers.Length) {
				if(autoTest) {
					if(Random.Range(0, 2) == 1) {
						float value = Random.Range(-10f, 10f);
						
						if(fm.flex > 90) {
							value = -Mathf.Abs(value);
						} else if(fm.flex < -20) {
							value = Mathf.Abs(value);
						}

						fm.flex += value;
					}

					bool change = true;
					Vector3 v = Vector3.zero;

					switch(Random.Range(0, 8)) {
						case 0:
							v += Vector3.right;
							break;
						case 1:
							v += Vector3.up;
							break;
						case 2:
							v += Vector3.forward;
							break;
						default:
							change = false;
							break;
					}
					
					if(change) {
						Vector3 initial = fm.rotation;
						fm.rotation += v * Random.Range(-6f, 6f);

						float max = 30;
						if(Mathf.Abs(fm.rotation.x) > max || Mathf.Abs(fm.rotation.y) > max || Mathf.Abs(fm.rotation.z) > max) {
							fm.rotation = initial;
						}
					}

					testers[i] = fm;
				}

				if(i == 0) hand.thumb.flex = fm.flex;
				else if(i == 1) hand.index.flex = fm.flex;
				else if(i == 2) hand.middle.flex = fm.flex;
				else if(i == 3) hand.ring.flex = fm.flex;
				else if(i == 4) hand.pinky.flex = fm.flex;

				hand.MoveFinger(ref fingers[fm.fingerNumber], new FitrQuaternion(Quaternion.Euler(fm.rotation)), fm.flex);
			}
		}
	}
}