using System.Collections.Generic;
using System.Linq;
using UnityEngine;

[System.Serializable]
public class FitrQuaternion {
    public Transform offset;

    [System.NonSerialized]
    public bool hasOrigin;
    private Quaternion actual = Quaternion.identity, origin = Quaternion.identity, offsetOrigin = Quaternion.identity;

    public FitrQuaternion(Quaternion q) {
        v = q;
    }

    public Quaternion v {
        set {
            actual = new Quaternion(value.x, value.y, value.w, value.z);
            CheckOrigin();

            if(hasOrigin && offset != null) {
                //(palm.y - finger.y)
                Vector3 of = Vector3.up * (offsetOrigin.eulerAngles - origin.eulerAngles).y;
                //Debug.Log(origin.eulerAngles + " and " + offsetOrigin.eulerAngles + " = " + of);
                offset.localEulerAngles = of;
            }
        }

        get {
            return actual;
        }
    }

    public Quaternion o {
        set {
            offsetOrigin = value;
        }

        get {
            return origin;
        }
    }

    public Quaternion d {
        set {
            actual = value;
            CheckOrigin();
        }

        get {
            return actual;
        }
    }

    void CheckOrigin() {
        if(!hasOrigin) {
            origin = actual;
            hasOrigin = true;
        }
    }
}