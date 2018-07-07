using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class InfinityGauntlet : MonoBehaviour {
	public GameObject head;
	public InfinityStone[] stones;

	void Start () {
		foreach(InfinityStone stone in stones) {
			stone.gauntlet = this;
		}
	}
}
