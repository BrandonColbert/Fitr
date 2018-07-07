using System.Collections.Generic;
using System.Linq;
using UnityEngine;

public class StoneTime : InfinityStone {
    [System.Serializable]
    public struct TemporalData {
        public bool destroyed, rigidbody;
        public int destroyFramesLeft;

        public Vector3 position, velocity, angularVelocity;
        public Quaternion rotation;

        public TemporalData(bool d, int dfl, Vector3 p, Quaternion r) : this(d, dfl, p, r, false, Vector3.zero, Vector3.zero) {}

        public TemporalData(bool d, int dfl, Vector3 p, Quaternion r, bool rb, Vector3 v, Vector3 av) {
            destroyed = d;
            destroyFramesLeft = dfl;
            position = p;
            rotation = r;
            rigidbody = rb;
            velocity = v;
            angularVelocity = av;
        }
    }

    public class TemporalManipulator : MonoBehaviour {
        [SerializeField]
        public List<TemporalData> data;
        private Rigidbody rb;
        public bool destroyed;
        public int destroyFramesLeft;

        void Start() {
            if(data == null) data = new List<TemporalData>();
            rb = GetComponent<Rigidbody>();
            if(timeStopped) Freeze();
        }

        public void FixedUpdate() {
            rb = GetComponent<Rigidbody>();

            if(timeStopped) {
                if(scrubPoint < data.Count) {
                    TemporalData d = data[Mathf.RoundToInt(scrubPoint)];

                    destroyed = d.destroyed;
                    destroyFramesLeft = d.destroyFramesLeft;

                    transform.localPosition = d.position;
                    transform.localRotation = d.rotation;

                    if(d.rigidbody) {
                        if(!rb) {
                            rb = gameObject.AddComponent<Rigidbody>();
                            rb.isKinematic = true;
                        }

                        rb.velocity = d.velocity;
                        rb.angularVelocity = d.angularVelocity;
                    } else {
                        if(rb) {
                            Destroy(rb);
                            rb = null;
                        }
                    }

                    gameObject.SetActive(!destroyed);
                }
            } else {
                UnFreeze();
                data.Insert(0, rb ? new TemporalData(destroyed, destroyFramesLeft, transform.localPosition, transform.localRotation, true, rb.velocity, rb.angularVelocity) :
                                    new TemporalData(destroyed, destroyFramesLeft, transform.localPosition, transform.localRotation));

                if(data.Count > timePoints) {
                    data.RemoveAt(data.Count - 1);

                    if(destroyed) {
                        destroyFramesLeft--;
                        if(destroyFramesLeft <= 0) Destroy(gameObject);
                    }
                }
            }
        }

        void OnDestroy() {
            temporalContent.Remove(this);

            if(!destroyed && transform.parent == null) {
                GameObject save = Instantiate(gameObject);
                save.name = name;
                save.SetActive(false);

                TemporalManipulator tm = save.GetComponent<TemporalManipulator>();
                tm.enabled = true;
                tm.destroyed = true;
                tm.destroyFramesLeft = timePoints;
                temporalContent.Add(tm);
            }
        }

        void OnApplicationQuit() {
            destroyed = true;
        }

        public virtual void Freeze() {
            if(rb) rb.isKinematic = true;
        }

        public virtual void UnFreeze() {
            if(rb) rb.isKinematic = false;
        }
    }

    public class FixedTemporalObject : MonoBehaviour {}

    private static List<TemporalManipulator> temporalContent;
    private static bool timeStopped;
    private static float scrubPoint;
    private static int timePoints;
    private int scrubTarget;

    public int rewindSeconds = 5;
    public float timeShiftSpeed = 100f;
    public bool instantTimeShift = true;

    public override void Start() {
        base.Start();
        temporalContent = new List<TemporalManipulator>();

        Transform p = transform;
        while(p.parent) p = p.parent;
        SetFixed(p);

        GetTemporalObjects();
    }

    void SetFixed(Transform t) {
        t.gameObject.AddComponent<FixedTemporalObject>();

        foreach(Transform c in t) {
            SetFixed(c);
        }
    }

    protected override void FixedUpdate() {
        base.FixedUpdate();
        timeStopped = inUse;

        if(!inUse) {
            timePoints = (int)Mathf.Round(rewindSeconds / Time.fixedDeltaTime);
        }

        for(int i = 0; i < temporalContent.Count; i++) {
            TemporalManipulator t = temporalContent[i];
            if(t) {
                if(t.destroyed) t.FixedUpdate();
            } else {
                temporalContent.Remove(t);
            }
        }
    }

    public void GetTemporalObjects() {
        GameObject[] objects = UnityEngine.Object.FindObjectsOfType<GameObject>().Where(o => 
            o.activeInHierarchy && !o.GetComponent<TemporalManipulator>() && !o.GetComponent<FixedTemporalObject>()
        ).ToArray();

        foreach(GameObject o in objects) {
            temporalContent.Add(o.AddComponent<TemporalManipulator>());
        }
    }

    ///<summary>From 0 (now) to 1 (then)</summary>
    public void Scrub(float point) {
        scrubTarget = (int)Mathf.Round(Mathf.Clamp(point, 0f, 1f) * timePoints);
    }
    
    protected override void DuringUse() {
        int change = scrubPoint == scrubTarget ? 0 : scrubPoint < scrubTarget ? 1 : -1;

        if(change != 0) {
            if(instantTimeShift) {
                scrubPoint = scrubTarget;
            } else {
                scrubPoint += change * timeShiftSpeed * Time.deltaTime;
                if((change > 0 && scrubPoint > scrubTarget) || (change < 0 && scrubPoint < scrubTarget)) scrubPoint = scrubTarget;
            }
        }
    }

    protected override void OnActivate() {
        scrubPoint = scrubTarget = 0;
        foreach(TemporalManipulator o in temporalContent) o.Freeze();
    }

    protected override void OnDeactivate() {
        foreach(TemporalManipulator o in temporalContent) o.UnFreeze();
    }
}