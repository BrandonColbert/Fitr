using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class InfinityGauntlet : MonoBehaviour {
	public FitrGestures gestures;
	public GameObject head;
	public InfinityStone[] stones;

	void Start () {
		foreach(InfinityStone stone in stones) {
			stone.gauntlet = this;
		}
	}

	void Update() {
		if(gestures.current == FitrGestures.Gesture.FIST && gestures.last != FitrGestures.Gesture.FIST) {
			Infinity<StonePower>().Toggle();
		}
	}

	InfinityStone Infinity<T>() {
		foreach(InfinityStone stone in stones) if(stone.GetType() == typeof(T)) return stone;
		return null;
	}
}
