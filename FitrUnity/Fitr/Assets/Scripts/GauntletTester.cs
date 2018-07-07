using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GauntletTester : MonoBehaviour {
    public Texture2D crosshairImage;
	public StoneTime timeStone;

    private float scrub;
    private bool active;
    private bool wasActive;
    
    public StoneSpace spaceStone;
    public StoneMind mindStone;
    public StonePower powerStone;
    public StoneReality realityStone;
    
	void Update () {
        if(timeStone) {
            if(Input.GetKeyDown(KeyCode.F)) {
                active = !active;
            }

            float speed = 0.1f * Time.deltaTime;
            if(Input.GetKey(KeyCode.Q)) {
                scrub -= speed;
                if(scrub < 0f) scrub = 0f;
            } else if(Input.GetKey(KeyCode.E)) {
                scrub += speed;
                if(scrub > 1f) scrub = 1f;
            }

            if(!wasActive && active) {
                scrub = 0;
                timeStone.Toggle(true);
            } else if(wasActive && !active) {
                timeStone.Toggle(false);
            }

            wasActive = active;

            if(active) {
                timeStone.Scrub(scrub);
            }
        }

        if(spaceStone) {
            if(Input.GetKeyDown(KeyCode.V)) {
                spaceStone.Toggle(true);
            }

            if(Input.GetKeyUp(KeyCode.V)) {
                spaceStone.Toggle(false);
            }
        }

        if(mindStone) {
            if(Input.GetKeyDown(KeyCode.R)) {
                mindStone.Toggle(true);
            }

            if(Input.GetKeyUp(KeyCode.R)) {
                mindStone.Toggle(false);
            }
        }

        if(powerStone) {
            if(Input.GetMouseButtonDown(0)) {
                powerStone.Toggle();
            }
        }

        if(realityStone) {
            if(Input.GetKeyDown(KeyCode.C)) {
                realityStone.Toggle();
            }
        }
	}

    void OnGUI() {
        float scale = 0.015f;

        GUI.DrawTexture(new Rect(Screen.width / 2f - crosshairImage.width * scale / 2f,
                                 Screen.height / 2f - crosshairImage.height * scale / 2f,
                                 crosshairImage.width * scale, crosshairImage.height * scale), crosshairImage);
    }
}