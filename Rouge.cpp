#include "StdAfx.h"
#include "Rouge.h"
#include <algorithm>    // std::max
#include <iostream>     // std::cout

CRouge::CRouge(void)
{

}


CRouge::~CRouge(void)
{
}

//[in]nGram: N-gram 
//[in]bSeg: Segment
double CRouge::Rouge_N(std::wstring wstrCandidate,std::vector<std::wstring> vecReference, int nGram,BOOL bSeg)
{
	double dbScore(0.0),t_dbScore(0.0);
	double dbCount_Denominator(0.0),dbCount_numerator(0.0);//��ĸ������
	int nN = nGram;
	size_t pos(0), pos1(0),pos2(0),pos3(0);
	if (bSeg)//��Ϊ��λ
	{
		if (vecReference.size() > 0)
		{
			for(int nRIndex=0; nRIndex < (int)vecReference.size(); nRIndex++)
			{
				std::wstring wstrReference = vecReference.at(nRIndex);
				if (wstrReference.size() >= nGram)
				{
					while((pos1 = wstrReference.find(L" ",pos)) != std::string::npos)
					{

						size_t t_pos = wstrReference.substr(pos,pos1-pos).find(L"/w",0);
						if (t_pos != std::string::npos)
						{
							pos = pos1+1;
							nN = nGram;
							continue;
						}
						++dbCount_Denominator;
						--nN;
						if (nN > 0)
						{
							pos2 = pos1+1;
							for (;nN > 0;nN--)
							{
								
								pos3 = wstrReference.find(L" ",pos2);
								if (pos3 < 0)
									break;
								pos2 = pos3+1;
							}
							if (nN > 0)
								break;

						}
						else
						{
							pos3 = pos1;
						}
						std::wstring  wstrSubRef = wstrReference.substr(pos,pos3-pos);
						if (wstrSubRef.find(L"/w",0) != std::string::npos)
						{
							pos = pos1+1;
							nN = nGram;
							continue;
						}
						pos3 = pos2 = 0;
						if((pos3 = wstrCandidate.find(wstrSubRef,pos2)) !=std::string::npos)//���ִ���
						{
							++dbCount_numerator;
						//	pos2 = pos3+wstrSubRef.size();
						}

						pos = pos1+1;
						nN = nGram;
					}
					if (dbCount_Denominator > 0)
					{
						t_dbScore = dbCount_numerator/dbCount_Denominator;
						if (t_dbScore > dbScore)
							dbScore = t_dbScore;
						dbCount_numerator = 0;
						dbCount_Denominator = 0;
					}

				}
			}
		}
	}
	else
	{//��Ϊ��λ

		if (vecReference.size() > 0)
		{
			for(int nRIndex=0; nRIndex < (int)vecReference.size(); nRIndex++)
			{
				std::wstring wstrReference = vecReference.at(nRIndex);
				if (wstrReference.size() >= nGram)
				{
					dbCount_Denominator = (wstrReference.size()-nGram+1);
					pos = pos1 = 0;
					for(int nCIndex=0; nCIndex < wstrReference.size()-nGram+1; nCIndex++)
					{
						std::wstring  wstrSubRef = wstrReference.substr(nCIndex,nGram);
						if((pos1 = wstrCandidate.find(wstrSubRef,pos)) !=std::string::npos)//����������
						{
							++dbCount_numerator;
					//		pos = pos1+1;
						}
					}

					if (dbCount_Denominator > 0)
					{
						t_dbScore = dbCount_numerator/dbCount_Denominator;
						if (t_dbScore > dbScore)
							dbScore = t_dbScore;
						dbCount_numerator = 0;
						dbCount_Denominator = 0;
					}
				}

			}
		}
	}
	
	return dbScore;
}


double CRouge::Rouge_L(std::wstring wstrCandidate,std::vector<std::wstring> vecReference,BOOL bSeg)
{
	double dbScore(0.0),t_dbScore(0.0);
	double dbCount_Denominator(0.0),dbCount_numerator(0.0);//��ĸ������
	
	size_t pos(0), pos1(0),pos2(0),pos3(0);
	if (bSeg)//��Ϊ��λ
	{
		if (vecReference.size() > 0)
		{
			for(int nRIndex=0; nRIndex < (int)vecReference.size(); nRIndex++)
			{
				std::wstring wstrReference = vecReference.at(nRIndex);

			//	t_dbScore = ComputeFlcs_Sentence(wstrCandidate,wstrReference,bSeg);
				t_dbScore = ComputeFlcs_Summary(wstrCandidate,wstrReference,bSeg);
				if (t_dbScore > dbScore)
					dbScore = t_dbScore;

			}
		}

	}
	else
	{
		if (vecReference.size() > 0)
		{
			for(int nRIndex=0; nRIndex < (int)vecReference.size(); nRIndex++)
			{
				std::wstring wstrReference = vecReference.at(nRIndex);
			//	t_dbScore = ComputeFlcs_Sentence(wstrCandidate,wstrReference,bSeg);
				t_dbScore = ComputeFlcs_Summary(wstrCandidate,wstrReference,bSeg);
				
				if (t_dbScore > dbScore)
					dbScore = t_dbScore;

				
			}
		}
	}

	return dbScore;
}

double CRouge::NormalizedPairwiseLCS(std::wstring wstrCandidate,std::vector<std::wstring> vecReference,BOOL bSeg)
{
	double dbScore(0.0),t_dbScore(0.0);
	double dbCount_Denominator(0.0),dbCount_numerator(0.0);//��ĸ������
	
	size_t pos(0), pos1(0),pos2(0),pos3(0);
	if (bSeg)//��Ϊ��λ
	{
		if (vecReference.size() > 0)
		{
			for(int nRIndex=0; nRIndex < (int)vecReference.size(); nRIndex++)
			{
				std::wstring wstrReference = vecReference.at(nRIndex);

				t_dbScore = ComputeFlcs_NormalizedPairwiseLCS(wstrCandidate,wstrReference,bSeg);
	
				if (t_dbScore > dbScore)
					dbScore = t_dbScore;

			}
		}

	}
	else
	{
		if (vecReference.size() > 0)
		{
			for(int nRIndex=0; nRIndex < (int)vecReference.size(); nRIndex++)
			{
				std::wstring wstrReference = vecReference.at(nRIndex);
				t_dbScore = ComputeFlcs_NormalizedPairwiseLCS(wstrCandidate,wstrReference,bSeg);
		
				
				if (t_dbScore > dbScore)
					dbScore = t_dbScore;

				
			}
		}
	}

	return dbScore;
}
//compute LCS-base F-measure
//�������
double  CRouge::ComputeFlcs_Sentence(std::wstring &wstrCandidate,
	std::wstring &wstrReference,
	BOOL bSeg)
{
	double dbFlcs(0.0);
	int nLCS(0),nR(0),nC(0);
	GetLCS(nLCS,nR,nC,wstrCandidate,wstrReference,bSeg);
	double Rlcs = static_cast<double>(nLCS)/static_cast<double>(nR);
	double Plcs = static_cast<double>(nLCS)/static_cast<double>(nC);
	double beta = Plcs/Rlcs;
	dbFlcs = ((1.0 + beta*beta)*Rlcs*Plcs)/(Rlcs + beta*beta*Plcs);
/*	int **pTable,nrowSize(0),ncolumnSize(0);
	std::vector<std::wstring>  m_vectCandidate,m_vectReference;
	std::wstring wstrTemp(L""),wstrPunctuation(L"���������� ");
	if (bSeg)///segment
	{
		std::size_t pos1(0),pos(0);
		while((pos1 = wstrCandidate.find(L" ",pos)) != std::string::npos)
		{

			size_t t_pos = wstrCandidate.substr(pos,pos1-pos).find(L"/w",0);
			if (t_pos != std::string::npos)
			{
				pos = pos1+1;

				continue;
			}
			wstrTemp = wstrCandidate.substr(pos,pos1-pos);

			m_vectCandidate.push_back(wstrTemp);
			if (pos == 0)
			{
				m_vectCandidate.push_back(wstrTemp);
			}
			pos = pos1+1;
		}

		pos1 = 0; 
		pos = 0;

		while((pos1 = wstrReference.find(L" ",pos)) != std::string::npos)
		{

			size_t t_pos = wstrReference.substr(pos,pos1-pos).find(L"/w",0);
			if (t_pos != std::string::npos)
			{
				pos = pos1+1;

				continue;
			}
			wstrTemp = wstrReference.substr(pos,pos1-pos);

			m_vectReference.push_back(wstrTemp);
			if (pos == 0)
			{
				m_vectReference.push_back(wstrTemp);
			}
			pos = pos1+1;
		}


	}
	else
	{

		wstrTemp = wstrCandidate.at(0);
		m_vectCandidate.push_back(wstrTemp);
		for(std::vector<std::wstring>::size_type ix=0; ix != wstrCandidate.size(); ++ix)
		{
			wstrTemp = wstrCandidate.at(ix);
			if (wstrPunctuation.find(wstrTemp,0) != std::string::npos || wstrTemp.at(0) == 13)
				continue;
			m_vectCandidate.push_back(wstrTemp);
		}
		wstrTemp = wstrReference.at(0);
		m_vectReference.push_back(wstrTemp);
		for(std::vector<std::wstring>::size_type ir=0; ir != wstrReference.size(); ++ir)
		{
			wstrTemp = wstrReference.at(ir);
			if (wstrPunctuation.find(wstrTemp,0) != std::string::npos || wstrTemp.at(0) == 13)
				continue;
			m_vectReference.push_back(wstrTemp);
		}


	}

	nrowSize = m_vectCandidate.size();
	ncolumnSize = m_vectReference.size();
	pTable = new int*[nrowSize];
	pTable[0] = new int[nrowSize*ncolumnSize];
	memset(pTable[0],0,nrowSize*ncolumnSize);
	for(int i = 1; i != nrowSize; i++)
	{
		pTable[i] = pTable[i-1]+ncolumnSize;
	}

	for (int nRow = 1; nRow != nrowSize; ++nRow)
	{
		for (int nColumn = 1; nColumn != ncolumnSize; ++nColumn)
		{
			if (m_vectCandidate.at(nRow) == m_vectReference.at(nColumn))
			{
				pTable[nRow][nColumn] = pTable[nRow-1][nColumn-1]+1;
			}
			else
			{
				if (pTable[nRow-1][nColumn] > pTable[nRow][nColumn-1])
					pTable[nRow][nColumn] = pTable[nRow-1][nColumn];
				else
					pTable[nRow][nColumn] =pTable[nRow][nColumn-1];
			}
		}
	}
	int nLCS = pTable[nrowSize-1][ncolumnSize-1];
	double Rlcs = static_cast<double>(nLCS)/static_cast<double>(nrowSize);
	double Plcs = static_cast<double>(nLCS)/static_cast<double>(ncolumnSize);
	double beta = Plcs/Rlcs;
	dbFlcs = ((1.0 + beta*beta)*Rlcs*Plcs)/(Rlcs + beta*beta*Plcs);
	if(pTable)
	{
		delete [](pTable[0]);
		delete []pTable;
		pTable = NULL;
	}*/
	return dbFlcs;

}
//��ժҪ����
//ROUGE-L takes the union LCS score
double CRouge::ComputeFlcs_Summary(std::wstring  &wstrCandidate,std::wstring  &wstrReference,BOOL bSeg)
{
	double dbFlcs(0.0);
	int nLCS(0),nR(0),nC(0);
	std::size_t pos(0),pos1(0),pos2(0),pos3(0);
	std::vector<std::wstring> vectUnionLCS;
	if (bSeg)
	{
		std::wstring  wstrCSentence(L""),wstrRSentence(L"");
		int t_nSubLCS(0),t_nLCS(0),t_nR(0),t_nC(0);
		while(((pos1 = wstrReference.find(L"/wj",pos)) != std::string::npos) || 
			((pos1 = wstrReference.find(L"/ww",pos)) != std::string::npos) ||
			((pos1 = wstrReference.find(L"/wt",pos)) != std::string::npos) ||
			((pos1 = wstrReference.find(L"/wf",pos)) != std::string::npos))
		{
			wstrRSentence = wstrReference.substr(pos, pos1-pos+4);
			pos2 = pos3 = 0;
			while(((pos3 = wstrCandidate.find(L"/wj",pos2)) != std::string::npos) || 
			((pos3 = wstrCandidate.find(L"/ww",pos2)) != std::string::npos) ||
			((pos3 = wstrCandidate.find(L"/wt",pos2)) != std::string::npos) ||
			((pos3 = wstrCandidate.find(L"/wf",pos2)) != std::string::npos))
			{
				wstrCSentence = wstrCandidate.substr(pos2,pos3-pos2+4);
				t_nLCS = 0,t_nR = 0,t_nC = 0;
				std::vector<std::wstring> t_LCS = GetLCS(t_nLCS,t_nR,t_nC,wstrCSentence,wstrRSentence,bSeg);
				//if (t_nSubLCS < t_nLCS)
				//	t_nSubLCS = t_nLCS;
				if (pos2 == 0)
					vectUnionLCS = t_LCS;
				else
				{
					for (std::vector<std::wstring>::iterator iter = t_LCS.begin(); iter != t_LCS.end(); ++iter)
					{
						if ( std::find(vectUnionLCS.begin(),vectUnionLCS.end(),*iter) == vectUnionLCS.end())
							vectUnionLCS.push_back(*iter);
					}
				}

				if (pos == 0)
					nC += t_nC;
				
				pos2 = pos3 + 4;
			}
			t_nSubLCS = vectUnionLCS.size();
			nR += t_nR;
			nLCS += t_nSubLCS;
			t_nSubLCS = 0,t_nLCS = 0,t_nR = 0,t_nC = 0;
			pos = pos1 + 4;

		}

	}
	else
	{
		std::wstring  wstrCSentence(L""),wstrRSentence(L"");
		int t_nSubLCS(0),t_nLCS(0),t_nR(0),t_nC(0);
		while(((pos1 = wstrReference.find(L"��",pos)) != std::string::npos) || 
			((pos1 = wstrReference.find(L"��",pos)) != std::string::npos) ||
			((pos1 = wstrReference.find(L"��",pos)) != std::string::npos) ||
			((pos1 = wstrReference.find(L"��",pos)) != std::string::npos))
		{
			wstrRSentence = wstrReference.substr(pos, pos1-pos);
			pos2 = pos3 = 0;
			while(((pos3 = wstrCandidate.find(L"��",pos2)) != std::string::npos) || 
			((pos3 = wstrCandidate.find(L"��",pos2)) != std::string::npos) ||
			((pos3 = wstrCandidate.find(L"��",pos2)) != std::string::npos) ||
			((pos3 = wstrCandidate.find(L"��",pos2)) != std::string::npos))
			{
				wstrCSentence = wstrCandidate.substr(pos2,pos3-pos2);
				t_nLCS = 0,t_nR = 0,t_nC = 0;
				GetLCS(t_nLCS,t_nR,t_nC,wstrCSentence,wstrRSentence,bSeg);
				if (t_nSubLCS < t_nLCS)
					t_nSubLCS = t_nLCS;
				if (pos == 0)
					nC += t_nC;
				
				pos2 = pos3 + 1;
			}
			nR += t_nR;
			nLCS += t_nSubLCS;
			t_nSubLCS = 0,t_nLCS = 0,t_nR = 0,t_nC = 0;
			pos = pos1 + 1;

		}

	}

	double Rlcs = static_cast<double>(nLCS)/static_cast<double>(nR);
	double Plcs = static_cast<double>(nLCS)/static_cast<double>(nC);
	double beta = Plcs/Rlcs;
	dbFlcs = ((1.0 + beta*beta)*Rlcs*Plcs)/(Rlcs + beta*beta*Plcs);
	return dbFlcs;

}

////Normalized Pairwise LCS take the bestLCS score
double CRouge::ComputeFlcs_NormalizedPairwiseLCS(std::wstring  &wstrCandidate,std::wstring  &wstrReference,BOOL bSeg)
{
	double dbFlcs(0.0);
	int nLCS(0),nR(0),nC(0);
	std::size_t pos(0),pos1(0),pos2(0),pos3(0);
	
	if (bSeg)
	{
		std::wstring  wstrCSentence(L""),wstrRSentence(L"");
		int t_nSubLCS(0),t_nLCS(0),t_nR(0),t_nC(0);
		while(((pos1 = wstrReference.find(L"/wj",pos)) != std::string::npos) || 
			((pos1 = wstrReference.find(L"/ww",pos)) != std::string::npos) ||
			((pos1 = wstrReference.find(L"/wt",pos)) != std::string::npos) ||
			((pos1 = wstrReference.find(L"/wf",pos)) != std::string::npos))
		{
			wstrRSentence = wstrReference.substr(pos, pos1-pos+4);
			pos2 = pos3 = 0;
			while(((pos3 = wstrCandidate.find(L"/wj",pos2)) != std::string::npos) || 
			((pos3 = wstrCandidate.find(L"/ww",pos2)) != std::string::npos) ||
			((pos3 = wstrCandidate.find(L"/wt",pos2)) != std::string::npos) ||
			((pos3 = wstrCandidate.find(L"/wf",pos2)) != std::string::npos))
			{
				wstrCSentence = wstrCandidate.substr(pos2,pos3-pos2+4);
				t_nLCS = 0,t_nR = 0,t_nC = 0;
				GetLCS(t_nLCS,t_nR,t_nC,wstrCSentence,wstrRSentence,bSeg);
				if (t_nSubLCS < t_nLCS)
					t_nSubLCS = t_nLCS;
				if (pos == 0)
					nC += t_nC;
				
				pos2 = pos3 + 4;
			}
			nR += t_nR;
			nLCS += t_nSubLCS;
			t_nSubLCS = 0,t_nLCS = 0,t_nR = 0,t_nC = 0;
			pos = pos1 + 4;

		}

	}
	else
	{
		std::wstring  wstrCSentence(L""),wstrRSentence(L"");
		int t_nSubLCS(0),t_nLCS(0),t_nR(0),t_nC(0);
		while(((pos1 = wstrReference.find(L"��",pos)) != std::string::npos) || 
			((pos1 = wstrReference.find(L"��",pos)) != std::string::npos) ||
			((pos1 = wstrReference.find(L"��",pos)) != std::string::npos) ||
			((pos1 = wstrReference.find(L"��",pos)) != std::string::npos))
		{
			wstrRSentence = wstrReference.substr(pos, pos1-pos);
			pos2 = pos3 = 0;
			while(((pos3 = wstrCandidate.find(L"��",pos2)) != std::string::npos) || 
			((pos3 = wstrCandidate.find(L"��",pos2)) != std::string::npos) ||
			((pos3 = wstrCandidate.find(L"��",pos2)) != std::string::npos) ||
			((pos3 = wstrCandidate.find(L"��",pos2)) != std::string::npos))
			{
				wstrCSentence = wstrCandidate.substr(pos2,pos3-pos2);
				t_nLCS = 0,t_nR = 0,t_nC = 0;
				GetLCS(t_nLCS,t_nR,t_nC,wstrCSentence,wstrRSentence,bSeg);
				if (t_nSubLCS < t_nLCS)
					t_nSubLCS = t_nLCS;
				if (pos == 0)
					nC += t_nC;
				
				pos2 = pos3 + 1;
			}
			nR += t_nR;
			nLCS += t_nSubLCS;
			t_nSubLCS = 0,t_nLCS = 0,t_nR = 0,t_nC = 0;
			pos = pos1 + 1;

		}

	}

	double Rlcs = static_cast<double>(nLCS)/static_cast<double>(nR);
	double Plcs = static_cast<double>(nLCS)/static_cast<double>(nC);
	double beta = 1.0;//Plcs/Rlcs;
	dbFlcs = ((1.0 + beta*beta)*Rlcs*Plcs)/(Rlcs + beta*beta*Plcs);
	return dbFlcs;
}

std::vector<std::wstring> CRouge::GetLCS(int &lcs,int &mR,int &nC,std::wstring  &wstrCandidate,std::wstring  &wstrReference,BOOL bSeg)
{
	double dbFlcs(0.0);
	int nrowSize(0),ncolumnSize(0); //**pTable,**prev,
	std::vector<std::wstring>  m_vectCandidate,m_vectReference,m_vectLCS;
	std::wstring wstrTemp(L""),wstrPunctuation(L"���������� ");
	if (bSeg)///segment
	{
		std::size_t pos1(0),pos(0);


		while((pos1 = wstrCandidate.find(L" ",pos)) != std::string::npos)
		{

			size_t t_pos = wstrCandidate.substr(pos,pos1-pos).find(L"/w",0);
			if (t_pos != std::string::npos)
			{
				pos = pos1+1;

				continue;
			}
			wstrTemp = wstrCandidate.substr(pos,pos1-pos);

			m_vectCandidate.push_back(wstrTemp);
			if (pos == 0)
			{
				m_vectCandidate.push_back(wstrTemp);
			}
			pos = pos1+1;
		}

		pos1 = 0; 
		pos = 0;

		while((pos1 = wstrReference.find(L" ",pos)) != std::string::npos)
		{

			size_t t_pos = wstrReference.substr(pos,pos1-pos).find(L"/w",0);
			if (t_pos != std::string::npos)
			{
				pos = pos1+1;

				continue;
			}
			wstrTemp = wstrReference.substr(pos,pos1-pos);

			m_vectReference.push_back(wstrTemp);
			if (pos == 0)
			{
				m_vectReference.push_back(wstrTemp);
			}
			pos = pos1+1;
		}


	}
	else
	{

		wstrTemp = wstrCandidate.at(0);
		m_vectCandidate.push_back(wstrTemp);
		for(std::vector<std::wstring>::size_type ix=0; ix != wstrCandidate.size(); ++ix)
		{
			wstrTemp = wstrCandidate.at(ix);
			if (wstrPunctuation.find(wstrTemp,0) != std::string::npos || wstrTemp.at(0) == 13)
				continue;
			m_vectCandidate.push_back(wstrTemp);
		}
		wstrTemp = wstrReference.at(0);
		m_vectReference.push_back(wstrTemp);
		for(std::vector<std::wstring>::size_type ir=0; ir != wstrReference.size(); ++ir)
		{
			wstrTemp = wstrReference.at(ir);
			if (wstrPunctuation.find(wstrTemp,0) != std::string::npos || wstrTemp.at(0) == 13)
				continue;
			m_vectReference.push_back(wstrTemp);
		}


	}

	nrowSize = m_vectCandidate.size();
	nC = nrowSize-1;
	ncolumnSize = m_vectReference.size();
	mR = ncolumnSize - 1;
	pTable = new int*[nrowSize];
	pTable[0] = new int[nrowSize*ncolumnSize];
	memset(pTable[0],0,nrowSize*ncolumnSize);
	for(int i = 1; i != nrowSize; i++)
	{
		pTable[i] = pTable[i-1]+ncolumnSize;
	}

	
	prev = new int*[nrowSize];//1:left top 2:left 3:top
	prev[0] = new int[nrowSize*ncolumnSize];
	memset(prev[0],0,nrowSize*ncolumnSize);
	for(int i = 1; i != nrowSize; i++)
	{
		prev[i] = prev[i-1]+ncolumnSize;
	}

	for (int nRow = 1; nRow != nrowSize; ++nRow)
	{
		for (int nColumn = 1; nColumn != ncolumnSize; ++nColumn)
		{
			if (m_vectCandidate.at(nRow) == m_vectReference.at(nColumn))
			{
				pTable[nRow][nColumn] = pTable[nRow-1][nColumn-1]+1;
				std::wstring temp = m_vectCandidate.at(nRow);
				prev[nRow][nColumn] = 1;
			}
			else
			{
				if (pTable[nRow-1][nColumn] > pTable[nRow][nColumn-1])
				{
					pTable[nRow][nColumn] = pTable[nRow-1][nColumn];
					prev[nRow][nColumn] = 3;
				}
				else
				{
					pTable[nRow][nColumn] =pTable[nRow][nColumn-1];
					prev[nRow][nColumn] = 2;
				}
			}
		}
	}
	lcs = pTable[nrowSize-1][ncolumnSize-1];

//	double Rlcs = static_cast<double>(nLCS)/static_cast<double>(nrowSize);
//	double Plcs = static_cast<double>(nLCS)/static_cast<double>(ncolumnSize);
//	double beta = Plcs/Rlcs;
//	dbFlcs = ((1.0 + beta*beta)*Rlcs*Plcs)/(Rlcs + beta*beta*Plcs);

	PrintLCS(m_vectCandidate,nrowSize-1,ncolumnSize-1,m_vectLCS);

	if(pTable)
	{
		delete [](pTable[0]);
		delete []pTable;
	//	pTable[0] = NULL;
	//	pTable = NULL;
	}
	if(prev)
	{
		delete [](prev[0]);
		delete []prev;
	//	prev[0] = NULL;
	//	prev = NULL;
	}

	return m_vectLCS;
}

void CRouge::PrintLCS(std::vector<std::wstring> vectorWords,int nRow,int nColumn,std::vector<std::wstring> &vectorLCS)
{
	if (nRow == 0 || nColumn == 0)
		return;
	std::wstring  wstrWord;
	if (prev[nRow][nColumn] == 1)
	{
		wstrWord = vectorWords.at(nRow);
		vectorLCS.push_back(wstrWord);
		PrintLCS(vectorWords,nRow-1,nColumn-1,vectorLCS);
	}
	else if (prev[nRow][nColumn] == 2)
	{
		PrintLCS(vectorWords,nRow,nColumn-1,vectorLCS);
	}
	else if (prev[nRow][nColumn] == 3)
	{
		PrintLCS(vectorWords,nRow-1,nColumn,vectorLCS);
	}
}