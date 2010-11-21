// OptionDialog.cpp : implementation file
//

#include "stdafx.h"
#include "FaceDetect.h"
#include "OptionDialog.h"
#include ".\optiondialog.h"

#include <vector>
using namespace std;
#include "IntImage.h"
#include "SimpleClassifier.h"
#include "AdaBoostClassifier.h"
#include "global.h"


// OptionDialog dialog

IMPLEMENT_DYNAMIC(OptionDialog, CDialog)
OptionDialog::OptionDialog(CWnd* pParent /*=NULL*/)
	: CDialog(OptionDialog::IDD, pParent)
	, m_ratio(1)
	, m_size(0)
	, m_fs(TRAIN_ADA)
	, m_lc(LC_ORIGINAL)
{
}

OptionDialog::~OptionDialog()
{
}

void OptionDialog::UpdateStatus()
{
	if(m_fs==TRAIN_ADA && m_ratio==1)
	{
		((CButton*)GetDlgItem(IDC_AdaBoost))->SetCheck(BST_CHECKED);
		((CButton*)GetDlgItem(IDC_AsymBoost))->SetCheck(BST_UNCHECKED);
		((CButton*)GetDlgItem(IDC_FFS))->SetCheck(BST_UNCHECKED);
		((CEdit*)GetDlgItem(IDC_RATIO))->EnableWindow(FALSE);
	}
	else if(m_fs==TRAIN_ADA && m_ratio>1)
	{
		((CButton*)GetDlgItem(IDC_AdaBoost))->SetCheck(BST_UNCHECKED);
		((CButton*)GetDlgItem(IDC_AsymBoost))->SetCheck(BST_CHECKED);
		((CButton*)GetDlgItem(IDC_FFS))->SetCheck(BST_UNCHECKED);
		((CEdit*)GetDlgItem(IDC_RATIO))->EnableWindow(TRUE);
	}
	else if(m_fs==TRAIN_FFS)
	{
		((CButton*)GetDlgItem(IDC_AdaBoost))->SetCheck(BST_UNCHECKED);
		((CButton*)GetDlgItem(IDC_AsymBoost))->SetCheck(BST_UNCHECKED);
		((CButton*)GetDlgItem(IDC_FFS))->SetCheck(BST_CHECKED);
		((CEdit*)GetDlgItem(IDC_RATIO))->EnableWindow(FALSE);
	}

	if(m_lc==LC_ORIGINAL)
		((CButton*)GetDlgItem(IDC_Original))->SetCheck(BST_CHECKED);
	else if(m_lc==LC_LAC)
		((CButton*)GetDlgItem(IDC_LAC))->SetCheck(BST_CHECKED);
	else if(m_lc==LC_FDA)
		((CButton*)GetDlgItem(IDC_FDA))->SetCheck(BST_CHECKED);
}

void OptionDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_RATIO, m_ratio);
	DDV_MinMaxDouble(pDX, m_ratio, 1, 2);
	DDX_Text(pDX, IDC_EDIT3, m_size);
	DDX_Text(pDX, IDC_EDIT3, m_size);
	UpdateStatus();
}


BEGIN_MESSAGE_MAP(OptionDialog, CDialog)
	ON_BN_CLICKED(IDC_AdaBoost, OnBnClickedAdaboost)
	ON_BN_CLICKED(IDC_AsymBoost, OnBnClickedAsymboost)
	ON_BN_CLICKED(IDC_FFS, OnBnClickedFfs)
	ON_BN_CLICKED(IDC_Original, OnBnClickedOriginal)
	ON_BN_CLICKED(IDC_LAC, OnBnClickedLac)
	ON_BN_CLICKED(IDC_FDA, OnBnClickedFda)
END_MESSAGE_MAP()


// OptionDialog message handlers

void OptionDialog::OnBnClickedAdaboost()
{
	m_fs = TRAIN_ADA;
	m_ratio = 1;
	((CEdit*)GetDlgItem(IDC_RATIO))->SetWindowText("1");
	((CEdit*)GetDlgItem(IDC_RATIO))->EnableWindow(FALSE);
}

void OptionDialog::OnBnClickedAsymboost()
{
	m_fs = TRAIN_ADA;
	m_ratio = 1.0268;
	((CEdit*)GetDlgItem(IDC_RATIO))->SetWindowText("1.0268");
	((CEdit*)GetDlgItem(IDC_RATIO))->EnableWindow(TRUE);
}

void OptionDialog::OnBnClickedFfs()
{
	m_fs = TRAIN_FFS;
	m_ratio = 1;
	((CEdit*)GetDlgItem(IDC_RATIO))->SetWindowText("1");
	((CEdit*)GetDlgItem(IDC_RATIO))->EnableWindow(FALSE);
}

void OptionDialog::OnBnClickedOriginal()
{
	m_lc = LC_ORIGINAL;
}

void OptionDialog::OnBnClickedLac()
{
	m_lc = LC_LAC;
}

void OptionDialog::OnBnClickedFda()
{
	m_lc = LC_FDA;
}
