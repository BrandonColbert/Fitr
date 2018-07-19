using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FitrGestures : MonoBehaviour {
	[System.Serializable]
	public enum Gesture {
		UNKNOWN,
		OPEN_HAND,
		FIST,
		POINT,
		OK,
		FLICK_START,
		FLICK_END,
		SNAP_START,
		SNAP_END
	}

	public FitrHand hand;
	public Gesture current = Gesture.UNKNOWN, last = Gesture.UNKNOWN;
	private Dictionary<Gesture, Func<FitrHand.Finger[], bool>> gestureMap;

	//Instead using floating sphere colliders that correspond to general regions of individual finger placements that light up when matching to form gestures

	void InitGestures() {
		if(gestureMap != null) {
			gestureMap.Clear();
		} else {
			gestureMap = new Dictionary<Gesture, Func<FitrHand.Finger[], bool>>();
		}

		gestureMap.Add(Gesture.OPEN_HAND, CheckOpenHand);
		gestureMap.Add(Gesture.FIST, CheckFist);
		gestureMap.Add(Gesture.POINT, CheckPoint);
		gestureMap.Add(Gesture.OK, CheckOk);
		gestureMap.Add(Gesture.FLICK_START, CheckFlickStart);
		gestureMap.Add(Gesture.FLICK_END, CheckFlickEnd);
		gestureMap.Add(Gesture.SNAP_START, CheckSnapStart);
		gestureMap.Add(Gesture.SNAP_END, CheckSnapEnd);
	}

	void Start() {
		InitGestures();
	}

	void OnValidate() {
		InitGestures();
	}

	void Update() {
		last = current;
		current = GetCurrentGesture();
	}

	Gesture GetCurrentGesture() {
		FitrHand.Finger[] fingers = hand.GetFingers();
		foreach(KeyValuePair<Gesture, Func<FitrHand.Finger[], bool>> pair in gestureMap) if(pair.Value(fingers)) return pair.Key;

		return Gesture.UNKNOWN;
	}

	bool CheckOpenHand(FitrHand.Finger[] fingers) {
		foreach(FitrHand.Finger finger in fingers) {
			if(finger.flex > 0.15f || Vector3.Dot(finger.a.transform.forward, Vector3.up) < 0.9f) {
				return false;
			}
		}

		return true;
	}

	bool CheckFist(FitrHand.Finger[] fingers) {
		for(int i = 0; i < fingers.Length; i++) {
			FitrHand.Finger finger = fingers[i];

			if(finger.flex < 0.75f) {
				return false;
			}
		}

		return true;
	}

	bool CheckPoint(FitrHand.Finger[] fingers) {
		for(int i = 0; i < fingers.Length; i++) {
			FitrHand.Finger finger = fingers[i];

			if(i == 1) {
				if(finger.flex > 0.2f) {
					return false;
				}
			} else {
				//CheckFist
				if(finger.flex < 0.75f) {
					return false;
				}
			}
		}

		return true;
	}

	bool CheckOk(FitrHand.Finger[] fingers) {
		for(int i = 0; i < fingers.Length; i++) {
			FitrHand.Finger finger = fingers[i];

			if(i == 0) {
				if(finger.a.transform.localEulerAngles.x > 340f || finger.a.transform.localEulerAngles.x < 280f || finger.flex < 0.1f) {
					return false;
				}
			} else if(i == 1) {
				if(finger.flex < 0.75f || Mathf.Abs(finger.a.transform.localEulerAngles.x) > 15f) {
					return false;
				}
			} else {
				//CheckOpen
				if(finger.flex > 0.15f || Mathf.Abs(finger.a.transform.localEulerAngles.x) > 15f) {
					return false;
				}
			}
		}

		return true;
	}

	bool CheckFlickStart(FitrHand.Finger[] fingers) {
		for(int i = 0; i < fingers.Length; i++) {
			FitrHand.Finger finger = fingers[i];

			if(i == 0) {
				if(finger.a.transform.localEulerAngles.x > 340f || finger.a.transform.localEulerAngles.x < 280f || finger.flex < 0.1f) {
					return false;
				}
			} else {
				//CheckFist
				if(finger.flex < 0.75f ||
					!(280f <= finger.a.transform.localEulerAngles.x && finger.a.transform.localEulerAngles.x <= 360f &&
					160f <= finger.a.transform.localEulerAngles.y && finger.a.transform.localEulerAngles.y <= 200f)) {
					return false;
				}
			}
		}

		return true;
	}

	bool CheckFlickEnd(FitrHand.Finger[] fingers) {
		for(int i = 0; i < fingers.Length; i++) {
			FitrHand.Finger finger = fingers[i];

			if(i == 0) {
				if(finger.a.transform.localEulerAngles.x > 340f || finger.a.transform.localEulerAngles.x < 280f || finger.flex < 0.1f) {
					return false;
				}
			} else if(i == 1) {
				//CheckOpen
				if(finger.flex > 0.15f || Mathf.Abs(finger.a.transform.localEulerAngles.x) > 15f) {
					return false;
				}
			} else {
				//CheckFist
				if(finger.flex < 0.75f ||
					!(280f <= finger.a.transform.localEulerAngles.x && finger.a.transform.localEulerAngles.x <= 360f &&
					160f <= finger.a.transform.localEulerAngles.y && finger.a.transform.localEulerAngles.y <= 200f)) {
					return false;
				}
			}
		}

		return true;
	}

	bool CheckSnapStart(FitrHand.Finger[] fingers) {
		return false;
	}

	bool CheckSnapEnd(FitrHand.Finger[] fingers) {
		return false;
	}
}
