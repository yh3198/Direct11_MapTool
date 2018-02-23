using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Camera_MainGame : MonoBehaviour
{
    // ============ variable ========== //
    private GameObject m_Player;
    private Vector3 m_CameraPos;
    // ================================ //

    // ============ Process ========== //
    void Start ()
    {
        m_Player = GameObject.FindGameObjectWithTag("Player");
        m_CameraPos = new Vector3();

    }
	
	void Update ()
    {
        m_CameraPos = m_Player.transform.position - m_Player.transform.forward.normalized * 3;
        m_CameraPos.y += 2.0f;
        transform.position = m_CameraPos;
        transform.LookAt(m_Player.transform.position);
    }
    // =============================== //
}
