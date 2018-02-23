using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.Events;
using UnityEngine.SceneManagement;

public class UI_Loading : MonoBehaviour
{
    // ============ variable ========== //
    public Button m_RightButton;
    public Button m_LeftButton;
    public Slider m_LoadingSlider;
    public GameObject[] m_HTPImage;
    
    private int m_HTPImagePos;
    private UnityAction RButtonAction = null;
    private UnityAction LButtonAction = null;
    // ================================ //

    // ============ Function ========== //
    public void LeftButtonClick()
    {
        if(m_HTPImagePos == 0)
        {
            return;
        }
        else
        {
            m_HTPImage[m_HTPImagePos].SetActive(false);
            m_HTPImagePos -= 1;
            m_HTPImage[m_HTPImagePos].SetActive(true);
        }
    }

    public void RightButtonClick()
    {
        if (m_HTPImagePos == 2)
        {
            return;
        }
        else
        {
            //Setting HTP Image
            m_HTPImage[m_HTPImagePos].SetActive(false);
            m_HTPImagePos += 1;
            m_HTPImage[m_HTPImagePos].SetActive(true);
        }
    }
    // =============================== //

    // ============ Process ========== //
    private void Awake()
    {
        //Load Slider Component
        m_LoadingSlider = gameObject.GetComponentInChildren<Slider>();

        //Setting Button Function
        RButtonAction = () => RightButtonClick();
        LButtonAction = () => LeftButtonClick();
        m_RightButton.onClick.AddListener(RButtonAction);
        m_LeftButton.onClick.AddListener(LButtonAction);
    }

    // Use this for initialization
    void Start ()
    {
        m_HTPImagePos = 0;

    }
	
	// Update is called once per frame
	void Update ()
    {
        if(m_LoadingSlider.value < 1)
        {
            m_LoadingSlider.value += (0.5f * Time.deltaTime);
        }
        else
        {
            if(Input.GetKeyDown(KeyCode.KeypadEnter))
            {
                SceneManager.LoadScene(2);
            }            
        }    
    }
    // =============================== //
}
