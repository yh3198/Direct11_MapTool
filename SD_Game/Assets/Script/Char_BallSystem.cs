using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Char_BallSystem : MonoBehaviour
{
    // ============ variable ========== //
    public GameObject Player;
    public GameObject RedBall;
    public GameObject BlueBall;
    public GameObject GreenBall;
    public float m_fBallDistance;
    public float m_fBallSpeed;
    public float m_fSkillBallDistance;
    public float m_fSkillBallSpeed;

    private Vector3 PlayerPos;
    // ================================ //

    // ============ Process ========== //
    // Use this for initialization
    void Start ()
    {
        RedBall.transform.localPosition = new Vector3(0.0f, 0.0f, m_fBallDistance);
        BlueBall.transform.localPosition = new Vector3(m_fBallDistance * 0.866f, 0.0f, -(m_fBallDistance * 0.5f));
        GreenBall.transform.localPosition = new Vector3(-(m_fBallDistance * 0.866f), 0.0f, -(m_fBallDistance * 0.5f));
        PlayerPos = new Vector3(0.0f, 0.5f, 0.0f);
    }
	
	// Update is called once per frame
	void Update ()
    {
        transform.position = Player.transform.position + PlayerPos;
        //transform.rotation = Player.transform.localRotation;

        // BallDistance up Skill
        if (Input.GetKey("z"))
        {
            RedBall.transform.localPosition = Vector3.Lerp(RedBall.transform.localPosition, new Vector3(0.0f, 0.0f, m_fSkillBallDistance), Time.deltaTime * 3.0f);
            BlueBall.transform.localPosition = Vector3.Lerp(BlueBall.transform.localPosition, new Vector3(m_fSkillBallDistance * 0.866f, 0.0f, -(m_fSkillBallDistance * 0.5f)), Time.deltaTime * 3.0f);
            GreenBall.transform.localPosition = Vector3.Lerp(GreenBall.transform.localPosition, new Vector3(-(m_fSkillBallDistance * 0.866f), 0.0f, -(m_fSkillBallDistance * 0.5f)), Time.deltaTime * 3.0f);
        }
        else
        {
            RedBall.transform.localPosition = Vector3.Lerp(RedBall.transform.localPosition, new Vector3(0.0f, 0.0f, m_fBallDistance), Time.deltaTime * 3.0f);
            BlueBall.transform.localPosition = Vector3.Lerp(BlueBall.transform.localPosition, new Vector3(m_fBallDistance * 0.866f, 0.0f, -(m_fBallDistance * 0.5f)), Time.deltaTime * 3.0f);
            GreenBall.transform.localPosition = Vector3.Lerp(GreenBall.transform.localPosition, new Vector3(-(m_fBallDistance * 0.866f), 0.0f, -(m_fBallDistance * 0.5f)), Time.deltaTime * 3.0f);
        }

        // RotationSpeed up Skill
        if (Input.GetKey("c"))
        {
            transform.RotateAround(transform.position, Vector3.up, m_fSkillBallSpeed * Time.deltaTime);
        }
        else
        {
            transform.RotateAround(transform.position, Vector3.up, m_fBallSpeed * Time.deltaTime);
        }
    }
    // =============================== //
}
