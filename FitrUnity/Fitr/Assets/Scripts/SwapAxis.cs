using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SwapAxis : MonoBehaviour {
    public bool removeScript = false;
    public bool swapXZ = false, swapXY = false, swapYZ = false, invertX = false, invertY = false, invertZ = false;
    private float px, pz, py, rx, rz, ry, sx, sz, sy;

    private void Get() {
        px = gameObject.transform.localPosition.x;
        pz = gameObject.transform.localPosition.z;
        py = gameObject.transform.localPosition.y;

        rx = gameObject.transform.localRotation.x;
        rz = gameObject.transform.localRotation.z;
        ry = gameObject.transform.localRotation.y;

        sx = gameObject.transform.localScale.x;
        sz = gameObject.transform.localScale.z;
        sy = gameObject.transform.localScale.y;
    }

    private void Set() {
        swapXY = false;
        swapXZ = false;
        swapYZ = false;
        invertX = false;
        invertY = false;
        invertZ = false;
    }

    private void Propogate() {
        for(int i = 0; i < transform.childCount; i++) {
            Transform t = transform.GetChild(i);
            SwapAxis p = t.gameObject.GetComponent<SwapAxis>();

            if(p == null) {
                p = t.gameObject.AddComponent<SwapAxis>();
            }

            p.removeScript = removeScript;
            p.swapXY = swapXY;
            p.swapXZ = swapXZ;
            p.swapYZ = swapYZ;
            p.invertX = invertX;
            p.invertY = invertY;
            p.invertZ = invertZ;
        }
    }

    void Update() {
        if(removeScript || swapXZ || swapXY || swapYZ || invertX || invertY || invertZ) {
            Get();

            if(swapXZ) {
                gameObject.transform.localPosition = new Vector3(pz, py, px);
                gameObject.transform.localEulerAngles = new Vector3(rz, ry, rx);
                gameObject.transform.localScale = new Vector3(sz, sy, sx);
            } else if(swapXY) {
                gameObject.transform.localPosition = new Vector3(py, px, pz);
                gameObject.transform.localEulerAngles = new Vector3(ry, rx, rz);
                gameObject.transform.localScale = new Vector3(sy, sx, sz);
            } else if(swapYZ) {
                gameObject.transform.localPosition = new Vector3(px, pz, py);
                gameObject.transform.localEulerAngles = new Vector3(rx, rz, ry);
                gameObject.transform.localScale = new Vector3(sx, sz, sy);
            } else if(invertX) {
                gameObject.transform.localPosition = new Vector3(-px, py, pz);
                gameObject.transform.localEulerAngles = new Vector3(-rx, ry, rz);
            } else if(invertY) {
                gameObject.transform.localPosition = new Vector3(px, -py, pz);
                gameObject.transform.localEulerAngles = new Vector3(rx, -ry, rz);
            } else if(invertZ) {
                gameObject.transform.localPosition = new Vector3(px, py, -pz);
                gameObject.transform.localEulerAngles = new Vector3(rx, ry, -rz);
            }

            Propogate();
            Set();

            if(removeScript) {
                Destroy(this);
            }
        }
    }
}