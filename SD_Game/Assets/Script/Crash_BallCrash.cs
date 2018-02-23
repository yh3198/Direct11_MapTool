using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Crash_BallCrash : MonoBehaviour
{
    // ============ variable ========== //
    public GameObject               m_Player;
    private Rigidbody               m_PlayerChar;
    private Rigidbody               m_EnemyChar;
    public float                    m_CrashPower;
    public Effect_EffectManager     m_EffectMgr;
    readonly string                 m_ball = "ball";
    // ================================ //

    // ============ Function ========== //
    private void CrashEffect(Collider other, bool Attack)
    {
        // 공격이펙트
        if (Attack)
        {
            int AECnt = m_EffectMgr.AttackEffect.Length;
            if (AECnt > 0)
            {
                for (int iArray = 0; iArray < AECnt; iArray++)
                {
                    if (!m_EffectMgr.AttackEffect[iArray].activeInHierarchy)
                    {
                        m_EffectMgr.AttackEffect[iArray].transform.position = other.transform.position;
                        m_EffectMgr.AttackEffect[iArray].SetActive(true);
                        m_EffectMgr.OffEffect(0, iArray, 1.1f);
                        return;
                    }
                }
            }
        }
        // 피격이펙트
        else
        {
            int DECnt = m_EffectMgr.DamagedEffect.Length;
            if (DECnt > 0)
            {
                for (int iDArray = 0; iDArray < DECnt; iDArray++)
                {
                    if (!m_EffectMgr.DamagedEffect[iDArray].activeInHierarchy)
                    {
                        m_EffectMgr.DamagedEffect[iDArray].transform.position = other.transform.position;
                        m_EffectMgr.DamagedEffect[iDArray].SetActive(true);
                        m_EffectMgr.OffEffect(1, iDArray, 1.1f);
                        return;
                    }
                }
            }
        }
    }
    // =============================== //

    // ============ Process ========== //
    // Use this for initialization
    void Start()
    {
        m_PlayerChar = m_Player.GetComponent<Rigidbody>();
    }

    // Update is called once per frame (now don't use it)
    //void Update()
    //{
    //}

    private void OnTriggerEnter(Collider other)
    {
        if (other.CompareTag(m_ball))
        {
            m_EnemyChar = other.transform.parent.gameObject.transform.parent.gameObject.GetComponentInParent<Rigidbody>();
            Vector3 vPlayer = m_Player.transform.position;
            // 충돌체(구)의 부모(구의 집합체)의 부모(충돌한 구를 가지고 있는 캐릭터)의 포지션
            Vector3 vEnemy = other.transform.parent.gameObject.transform.parent.gameObject.transform.position;
            vPlayer.y = 0.0f;
            vEnemy.y = 0.0f;
            Vector3 CrashDir = (vPlayer - vEnemy).normalized;
            switch (this.name)
            {
                case "Attack_Red":
                    {
                        if (other.name == "Attack_Blue")
                        {
                            CrashEffect(other, false);
                            m_PlayerChar.velocity += (CrashDir * m_CrashPower * 1.5f);
                        }
                        else if (other.name == "Attack_Red")
                        {
                            CrashEffect(other, false);
                            m_PlayerChar.velocity += (CrashDir * m_CrashPower);
                            m_EnemyChar.velocity -= (CrashDir * m_CrashPower * 0.5f);
                        }
                        else
                        {
                            CrashEffect(other, true);
                            m_EnemyChar.velocity -= (CrashDir * m_CrashPower);
                        }
                        break;
                    }
                case "Attack_Blue":
                    {
                        if (other.name == "Attack_Blue")
                        {
                            CrashEffect(other, false);
                            m_PlayerChar.velocity += (CrashDir * m_CrashPower * 0.5f);
                            m_EnemyChar.velocity -= (CrashDir * m_CrashPower * 0.5f);
                        }
                        else if (other.name == "Attack_Red")
                        {
                            CrashEffect(other, true);
                            m_EnemyChar.velocity -= (CrashDir * m_CrashPower);
                        }
                        else
                        {
                            CrashEffect(other, false);
                            m_PlayerChar.velocity += (CrashDir * m_CrashPower);
                        }
                        break;
                    }
                case "Attack_Green":
                    {
                        if (other.name == "Attack_Blue")
                        {
                            CrashEffect(other, true);
                            m_EnemyChar.velocity -= (CrashDir * m_CrashPower);
                        }
                        else if (other.name == "Attack_Red")
                        {
                            CrashEffect(other, false);
                            m_PlayerChar.velocity += (CrashDir * m_CrashPower);
                        }
                        else
                        {
                            CrashEffect(other, false);
                            m_PlayerChar.velocity += (CrashDir * m_CrashPower * 0.5f);
                            m_EnemyChar.velocity -= (CrashDir * m_CrashPower * 0.5f);
                        }
                        break;
                    }
            }
        }
    }
    // =============================== //
}
