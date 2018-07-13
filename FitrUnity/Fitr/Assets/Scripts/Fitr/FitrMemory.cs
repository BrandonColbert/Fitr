using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.IO.MemoryMappedFiles;
using UnityEngine;

public class FitrMemory {
    public MemoryMappedFile map;
    public MemoryMappedViewStream sharedMemory;

    public int handSize;
    public string name;

    public FitrMemory(string name, int fingers) {
        this.name = name;
        handSize = (1 + fingers) * 4 + fingers;
    }

    public void Sync(FitrHand hand) {
        try {
            map = MemoryMappedFile.CreateOrOpen(name, handSize * sizeof(float));
            sharedMemory = map.CreateViewStream();

            float[] data = new float[handSize];
            byte[] buffer = new byte[handSize * sizeof(float)];
            sharedMemory.Read(buffer, 0, handSize * sizeof(float));
            Buffer.BlockCopy(buffer, 0, data, 0, handSize * sizeof(float));

            int index = 0;

            Func<float> nextFloat = () => {
                index++;
                return data[index - 1];
            };

            Func<Quaternion> nextQuaternion = () => {
                float x = nextFloat();
                float y = nextFloat();
                float z = nextFloat();
                float w = nextFloat();

                return new Quaternion(w, x, y, z);
            };

            hand.palmRotation.v = nextQuaternion();
            hand.thumb.flex = nextFloat();
            hand.thumb.rotation.v = nextQuaternion();
            hand.index.flex = nextFloat();
            hand.index.rotation.v = nextQuaternion();
            hand.middle.flex = nextFloat();
            hand.middle.rotation.v = nextQuaternion();
            hand.ring.flex = nextFloat();
            hand.ring.rotation.v = nextQuaternion();
            hand.pinky.flex = nextFloat();
            hand.pinky.rotation.v = nextQuaternion();
         } catch(Exception e) {
            Debug.Log(e.GetType().Name + ": " + e.Message);
        }
    }
}