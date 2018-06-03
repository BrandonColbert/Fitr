using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

public class CPPInterface : MonoBehaviour {
    [DllImport("FitrWin")]
    public static extern void Connect();

    [DllImport("FitrWin")]
    public static extern bool IsConnected();

	[DllImport("FitrWin")]
    public static extern void Disconnect();

	[DllImport("FitrWin")]
    public static extern IntPtr GetHand();

	[DllImport("FitrWin")]
    public static extern IntPtr GetFinger(IntPtr hand, int finger);

	[DllImport("FitrWin")]
    public static extern float GetFingerFlex(IntPtr finger);

	[DllImport("FitrWin")]
    public static extern IntPtr GetFingerRotation(IntPtr finger);

	[DllImport("FitrWin")]
    public static extern float GetQuaternionX(IntPtr rotation);

	[DllImport("FitrWin")]
    public static extern float GetQuaternionY(IntPtr rotation);

	[DllImport("FitrWin")]
    public static extern float GetQuaternionZ(IntPtr rotation);

	[DllImport("FitrWin")]
    public static extern float GetQuaternionW(IntPtr rotation);
}