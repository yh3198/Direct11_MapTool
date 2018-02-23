using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.Events;
using UnityEngine.SceneManagement;

public class UI_Login : MonoBehaviour
{
    // ============ variable ========== //
    public Button m_StartButtonDlg;         // 시작 버튼
    private UnityAction action = null;      // 시작 함수용 Action
    // =============================== //

    // ============ Function ========== //
    public void StartClick()
    {
        SceneManager.LoadScene(1);
    }
    // =============================== //

    // ============ Process ========== //
    private void Awake()
    {
        // 시작 함수 생성 및 컴포넌트 가져오기
        action = () => StartClick();
        m_StartButtonDlg = gameObject.GetComponentInChildren<Button>();
    }

    void Start () {
        // 리스너 추가(함수 추가)
        m_StartButtonDlg.onClick.AddListener(action);
	}
	
    //(now don't use it)
	//void Update () 
    //{
	//}
    // =============================== //
}
