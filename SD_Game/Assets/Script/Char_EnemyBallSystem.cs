using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Char_EnemyBallSystem : MonoBehaviour
{
    // ============ variable ========== //
    public GameObject Enemy;
    public GameObject RedBall;
    public GameObject BlueBall;
    public GameObject GreenBall;
    public float m_fBallDistance;
    public float m_fBallSpeed;

    private Vector3 EnemyPos;
    // ================================ //

    // ============ Process ========== //
    void Start ()
    {
        RedBall.transform.localPosition = new Vector3(0.0f, 0.0f, m_fBallDistance);
        BlueBall.transform.localPosition = new Vector3(m_fBallDistance * 0.866f, 0.0f, -(m_fBallDistance * 0.5f));
        GreenBall.transform.localPosition = new Vector3(-(m_fBallDistance * 0.866f), 0.0f, -(m_fBallDistance * 0.5f));
        EnemyPos = new Vector3(0.0f, 0.5f, 0.0f);
        m_fBallSpeed = Random.Range(50.0f, 200.0f);
    }
	
	void Update ()
    {
        transform.position = Enemy.transform.position + EnemyPos;
        transform.RotateAround(transform.position, Vector3.up, m_fBallSpeed * Time.deltaTime);
    }
    // =============================== //
}
