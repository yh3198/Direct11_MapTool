using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Effect_EffectManager : MonoBehaviour
{
    // ============ variable ========== //
    public int AttackEffectCount;
    public int DamagedEffectCount;

    public GameObject[] AttackEffect;
    public GameObject[] DamagedEffect;

    readonly string AttackEffectString = "Prefabs/explode_Attack";
    readonly string DamagedEffectString = "Prefabs/explode_Damaged";
    // ================================ //

    // ============ Function ========== //
    public void OffEffect(int Effecttype, int EffectCnt, float time)
    {
        switch (Effecttype)
        {
            case 0 :
                StartCoroutine(OffAttackEffect(EffectCnt));
                break;
            case 1 :
                StartCoroutine(OffDamagedEffect(EffectCnt));
                break;
        }
    }

    IEnumerator OffAttackEffect(int EffectCnt)
    {
        yield return new WaitForSeconds(1.1f);
        AttackEffect[EffectCnt].SetActive(false);
        yield break;
    }
    IEnumerator OffDamagedEffect(int EffectCnt)
    {
        yield return new WaitForSeconds(1.1f);
        DamagedEffect[EffectCnt].SetActive(false);
        yield break;
    }
    // =============================== //

    // ============ Process ========== //
    // Use this for initialization
    void Start ()
    {
        GameObject GAttackEffect = Resources.Load(AttackEffectString, typeof(GameObject)) as GameObject;
        GameObject GDamagedEffect = Resources.Load(DamagedEffectString, typeof(GameObject)) as GameObject;
        AttackEffect = new GameObject[AttackEffectCount];
        DamagedEffect = new GameObject[DamagedEffectCount];
        for (int iAECnt = 0; iAECnt < AttackEffectCount; iAECnt++)
        {
            AttackEffect[iAECnt] = Instantiate(GAttackEffect, transform.position, transform.rotation);
            AttackEffect[iAECnt].SetActive(false);
        }
        for (int iDCnt = 0; iDCnt < DamagedEffectCount; iDCnt++)
        {
            DamagedEffect[iDCnt] = Instantiate(GDamagedEffect, transform.position, transform.rotation);
            DamagedEffect[iDCnt].SetActive(false);
        }
    }
	
	// Update is called once per frame (now don't use it)
	//void Update ()
    //{
    //}
    // =============================== //
}
