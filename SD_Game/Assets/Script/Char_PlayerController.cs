using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Char_PlayerController : MonoBehaviour
{
    // ============ variable ========== //
    // Control Property
    private Animator    m_Animator;      // 애니메이터 핸들
    private Rigidbody   m_Rigidbody;     // 물리 핸들

    // Character State variable
    private Vector3     m_moveDirection; // 움직임 방향 
    public float        m_speed;         // 움직임 속도
    public float        m_speedJump;     // 점프 속도
    public float        m_frontDir;      // 이동 방향 (앞, 뒤 판정용)
    private RaycastHit  m_Hitinfo;       // 충돌정보
    private bool        m_bJump;         // 점프 가능 판정값

    // Character ball State variable
    public float m_ballRadius;           // 캐릭터와 충돌구 사이의 거리
    // ================================ //


    // ============ Process ========== //
    // Use this for initialization
    void Start ()
    {
        m_moveDirection = new Vector3(0.0f, 0.0f, 0.0f);
        m_Animator = GetComponentInChildren<Animator>();
        m_Rigidbody = GetComponent<Rigidbody>();
        m_frontDir = 1.0f;
    }
	
	// Update is called once per frame
	void Update ()
    {
		// 이동
        if(Input.GetAxis("Vertical") == 0.0f)
        {
            m_moveDirection.z = 0.0f;
        }
        else
        {
            m_moveDirection.z = Input.GetAxis("Vertical") * m_speed;
        }

        // 방향전환
        float CharAxis = Input.GetAxis("Horizontal");
        if(CharAxis != 0.0f)
        {
            transform.Rotate(0, CharAxis * 2, 0);
        }

        // 지면에 있다면
        m_moveDirection.y = 0;
        // 점프
        if (m_bJump && Input.GetKey("x"))
        {
            m_moveDirection.y = m_speedJump;
            m_bJump = false;
        }

        // 이동 실행
        //Vector3 globalDirection = transform.TransformDirection(m_moveDirection);
        //m_Rigidbody.velocity = new Vector3(globalDirection.x, m_Rigidbody.velocity.y + m_moveDirection.y, globalDirection.z);
        this.transform.Translate(new Vector3(m_moveDirection.x, 0, m_moveDirection.z) * Time.deltaTime);
        m_Rigidbody.velocity += new Vector3 (0.0f, m_moveDirection.y, 0.0f);

        // 전진, 후진 방향 체크
        m_frontDir = 1.0f;
        if (m_moveDirection.z < 0)
        {
            m_moveDirection.z *= -1.0f;
            m_frontDir = -1.0f;
        }

        // 애니메이션 설정
        m_Animator.SetFloat("Move_Speed", m_moveDirection.z);
        m_Animator.SetFloat("Move_Horizon", CharAxis);
        // 뒤로 이동할 경우 애니메이션 역재생
        m_Animator.SetFloat("Move_FrontDir", m_frontDir); 
    }

    //점프 후 착지시 호출된다.
    private void OnCollisionEnter(Collision collision)
    {
        float CharDistance = 0.1f;
        if (Physics.Raycast(transform.position, Vector3.down, out m_Hitinfo, CharDistance))
        {
            m_bJump = true;
        }
    }
    // =============================== //
}
