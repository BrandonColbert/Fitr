using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class InfinityGauntlet : MonoBehaviour {
	public FitrGestures gestures;
	public GameObject head;
	public InfinityStone[] stones;
	public bool isSelectingStone;
	public InfinityStone selectedStone;

	void Start () {
		foreach(InfinityStone stone in stones) stone.gauntlet = this;
		selectedStone = stones[0];
	}

	void Update() {
		/*
		if(isSelectingStone) {
			if(gestures.current == FitrGestures.Gesture.FIST) {
				int sl = Mathf.RoundToInt(Mathf.Clamp((0.8f + Vector3.Dot(gestures.hand.palm.transform.forward, Vector3.right)) / 1.6f, 0f, 1f) * (stones.Length - 1));
				selectedStone = stones[sl];
				Debug.Log("Selecting stone: " + selectedStone.GetType());
			} else {
				isSelectingStone = false;
				Debug.Log("Stone selected is " + selectedStone.GetType());
			}
		} else {
			if(gestures.last != FitrGestures.Gesture.FIST && gestures.current == FitrGestures.Gesture.FIST && Vector3.Dot(gestures.hand.palm.transform.forward, Vector3.up) > 0.9f) {
				isSelectingStone = true;
				Debug.Log("Selecting stone");
			} else {
				if(selectedStone) ControlStones();
			}
		}
		*/
	}

	void ControlStones() {
		if(IsInfinity<StoneTime>()) {
			
		} else if(IsInfinity<StoneSpace>() || IsInfinity<StoneMind>()) {
			if(gestures.last != FitrGestures.Gesture.POINT && gestures.current == FitrGestures.Gesture.POINT) {
				//selectedStone.Toggle(true);
				Debug.Log("Toggling on " + selectedStone.GetType());
			} else if(gestures.last == FitrGestures.Gesture.POINT && gestures.current != FitrGestures.Gesture.POINT) {
				//selectedStone.Toggle(false);
				Debug.Log("Toggling off " + selectedStone.GetType());
			}
		} else if(IsInfinity<StonePower>() || IsInfinity<StoneSoul>()) {
			if(gestures.last != FitrGestures.Gesture.FIST && gestures.current == FitrGestures.Gesture.FIST) {
				//selectedStone.Toggle();
				Debug.Log("Activating " + selectedStone.GetType());
			}
		} else if(IsInfinity<StoneReality>()) {
			if(gestures.last != FitrGestures.Gesture.POINT && gestures.current == FitrGestures.Gesture.POINT) {
				//selectedStone.Toggle();
				Debug.Log("Activating " + selectedStone.GetType());
			}
		}
	}

	bool IsInfinity<T>() {
		return selectedStone.GetType() == typeof(T);
	}

	InfinityStone Infinity<T>() {
		foreach(InfinityStone stone in stones) if(stone.GetType() == typeof(T)) return stone;
		return null;
	}
}