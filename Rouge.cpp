#include "StdAfx.h"
#include "Rouge.h"
#include <algorithm>    // std::max
#include <iostream>     // std::cout

CRouge::CRouge(void)
{
	m_dbScoreN1 = 0;
	m_dbScoreN2 = 0;
	m_dbScoreN3 = 0;
	m_dbScoreL = 0;
	m_dbScoreNPL = 0;
	m_dbScoreW  = 0;
	m_dbScoreS  = 0;
	m_dbScoreSU  = 0;
	m_dbTScoreSU = 0;
	m_dbTScoreNPL = 0;
	m_dbTScoreW = 0;
	m_dbScoreS = 0;
	m_dbScoreSU = 0;
	m_dbTScoreSU = 0;
	m_dbScoreC2 =  0;
	m_dbScoreC3 = 0;
	m_db2gram = 0;
	m_db3gram = 0;
	m_db4gram = 0;
}


CRouge::~CRouge(void)
{
}

//[in]nGram: N-gram 
//[in]bSeg: Segment
double CRouge::Rouge_N(std::wstring wstrCandidate,std::vector<std::wstring> vecReference, int nGram,BOOL bSeg)
{
	double dbScore(0.0),t_dbScore(0.0);
	double dbCount_Denominator(0.0),dbCount_numerator(0.0);//分母，分子
	int nN = nGram;
	size_t pos(0), pos1(0),pos2(0),pos3(0);
	if (bSeg)//词为单位
	{
		if (vecReference.size() > 0)
		{
			for(int nRIndex=0; nRIndex < (int)vecReference.size(); nRIndex++)
			{
				std::wstring wstrReference = vecReference.at(nRIndex);
				if ((int)wstrReference.size() >= nGram)
				{
					while((pos1 = wstrReference.find(L" ",pos)) != std::string::npos)
					{
#ifdef	__EXCLUDE_STOPWORDS
						if ((wstrReference.substr(pos,pos1-pos).find(L"/w",0) != std::string::npos) ||
							(wstrReference.substr(pos,pos1-pos).find(L"/e",0) != std::string::npos) ||
							(wstrReference.substr(pos,pos1-pos).find(L"/y",0) != std::string::npos) ||
							(wstrReference.substr(pos,pos1-pos).find(L"/o",0) != std::string::npos) ||
							(wstrReference.substr(pos,pos1-pos).find(L"/u",0) != std::string::npos) )

#else
						size_t t_pos = wstrReference.substr(pos,pos1-pos).find(L"/w",0);
						if (t_pos != std::string::npos)
#endif
						{
							pos = pos1+1;
							nN = nGram;
							continue;
						}
						
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
#ifdef	__EXCLUDE_STOPWORDS
						if ((wstrSubRef.find(L"/w",0) != std::string::npos) ||
							(wstrSubRef.find(L"/e",0) != std::string::npos) ||
							(wstrSubRef.find(L"/y",0) != std::string::npos) ||
							(wstrSubRef.find(L"/o",0) != std::string::npos) ||
							(wstrSubRef.find(L"/u",0) != std::string::npos) )

#else
						if (wstrSubRef.find(L"/w",0) != std::string::npos)
#endif
						{
							pos = pos1+1;
							nN = nGram;
							continue;
						}
						++dbCount_Denominator;
						pos3 = pos2 = 0;
						if((pos3 = wstrCandidate.find(wstrSubRef,pos2)) != std::string::npos)//出现次数
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
	{//字为单位

		if (vecReference.size() > 0)
		{
			for(int nRIndex=0; nRIndex < (int)vecReference.size(); nRIndex++)
			{
				std::wstring wstrReference = vecReference.at(nRIndex);
				if ((int)wstrReference.size() >= nGram)
				{
					dbCount_Denominator = (wstrReference.size()-nGram+1);
					pos = pos1 = 0;
					for(int nCIndex=0; nCIndex < (int)wstrReference.size()-nGram+1; nCIndex++)
					{
						std::wstring  wstrSubRef = wstrReference.substr(nCIndex,nGram);
						if (IsPunctuation(wstrSubRef))
						{
							--dbCount_Denominator;
							continue;
						}
						if((pos1 = wstrCandidate.find(wstrSubRef,pos)) !=std::string::npos)//出现最大次数
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

	if (nGram == 1)
	{
		if(bSeg)
		{

			m_dbScoreSN1 = m_dbScoreN1;
		}
		else
		{
			m_dbScoreSN1 = m_dbScoreON1;
		}
	}
	return dbScore;
}


void CRouge::Rouge_N_All(std::wstring wstrCandidate,std::vector<std::wstring> vecReference,BOOL bSeg)//2014.07.08
{
	double dbScore1(0.0),dbScore2(0.0),dbScore3(0.0),dbScore4(0.0),t_dbScore(0.0),a1(0),a2(0),a3(0);
	 m_dbScoreN1 = 0;
	 m_dbScoreN2 = 0;
	 m_dbScoreN3 = 0;
	 m_dbScoreN4 = 0;
	double dbCount_Denominator1(0.0),dbCount_numerator1(0.0),dbCount_Denominator2(0.0),dbCount_numerator2(0.0),
		dbCount_Denominator3(0.0),dbCount_numerator3(0.0),dbCount_Denominator4(0.0),dbCount_numerator4(0.0);//分母，分子
	int nN1 = 1,nN2 = 2,nN3 = 3,nN4 = 4,nGram1 = 1, nGram2 = 2, nGram3 = 3,nGram4 = 4;
	size_t pos(0), pos1(0),pos2(0),pos3(0);
	std::wstring  wstrSubRef1,wstrSubRef2,wstrSubRef3,wstrSubRef4;
	if (bSeg)//词为单位
	{
		if (vecReference.size() > 0)
		{
			for(int nRIndex=0; nRIndex < (int)vecReference.size(); nRIndex++)
			{
				std::wstring wstrReference = vecReference.at(nRIndex);
				if ((int)wstrReference.size() >= nGram1)
				{
					while((pos1 = wstrReference.find(L" ",pos)) != std::string::npos)
					{
#ifdef	__EXCLUDE_STOPWORDS
						if ((wstrReference.substr(pos,pos1-pos).find(L"/w",0) != std::string::npos) ||
							(wstrReference.substr(pos,pos1-pos).find(L"/e",0) != std::string::npos) ||
							(wstrReference.substr(pos,pos1-pos).find(L"/y",0) != std::string::npos) ||
							(wstrReference.substr(pos,pos1-pos).find(L"/o",0) != std::string::npos) ||
							(wstrReference.substr(pos,pos1-pos).find(L"/u",0) != std::string::npos) )

#else
						size_t t_pos = wstrReference.substr(pos,pos1-pos).find(L"/w",0);
						if (t_pos != std::string::npos)
#endif
						{
							pos = pos1+1;
							nN1 = nGram1;
							nN2 = nGram2;
							nN3 = nGram3;
							nN4 = nGram4;
							continue;
						}
						
						--nN1;
						--nN2;
						--nN3;
						--nN4;
						wstrSubRef1 = wstrReference.substr(pos,pos1-pos);
					//	if (nN3 > 0)
						if (nN4 > 0)
						{
							pos2 = pos1+1;
						//	for (;nN3 > 0;nN3--,nN2--)
							for (;nN4 > 0; nN4--,nN3--,nN2--)
							{
								
								if (nN2 == 0)
								{
									wstrSubRef2 = wstrReference.substr(pos,pos3-pos);
								}
								if (nN3 == 0)
								{
									wstrSubRef3 = wstrReference.substr(pos,pos3-pos);
								}
								pos3 = wstrReference.find(L" ",pos2);
								if (pos3 < 0)
									break;
								pos2 = pos3+1;
								
							}
							if (nN4> 0)
								break;

						}
						else
						{
							pos3 = pos1;
						}
						
						wstrSubRef4 = wstrReference.substr(pos,pos3-pos);
						////////N = 1
#ifdef	__EXCLUDE_STOPWORDS
						if ((wstrSubRef1.find(L"/w",0) != std::string::npos) ||
							(wstrSubRef1.find(L"/e",0) != std::string::npos) ||
							(wstrSubRef1.find(L"/y",0) != std::string::npos) ||
							(wstrSubRef1.find(L"/o",0) != std::string::npos) ||
							(wstrSubRef1.find(L"/u",0) != std::string::npos) )

#else
						if (wstrSubRef1.find(L"/w",0) != std::string::npos)
#endif
						{
							pos = pos1+1;
							nN1 = nGram1;
							nN2 = nGram2;
							nN3 = nGram3;
							nN4 = nGram4;
							continue;
						}
						++dbCount_Denominator1;
						pos3 = pos2 = 0;
						if((pos3 = wstrCandidate.find(wstrSubRef1,pos2)) != std::string::npos)//出现次数
						{
							++dbCount_numerator1;
						//	pos2 = pos3+wstrSubRef.size();
						}
						////////////////N=2
#ifdef	__EXCLUDE_STOPWORDS
						if ((wstrSubRef2.find(L"/w",0) != std::string::npos) ||
							(wstrSubRef2.find(L"/e",0) != std::string::npos) ||
							(wstrSubRef2.find(L"/y",0) != std::string::npos) ||
							(wstrSubRef2.find(L"/o",0) != std::string::npos) ||
							(wstrSubRef2.find(L"/u",0) != std::string::npos) )

#else
						if (wstrSubRef2.find(L"/w",0) != std::string::npos)
#endif
						{
							pos = pos1+1;
							nN1 = nGram1;
							nN2 = nGram2;
							nN3 = nGram3;
							nN4 = nGram4;
							continue;
						}
						++dbCount_Denominator2;
						pos3 = pos2 = 0;
						if((pos3 = wstrCandidate.find(wstrSubRef2,pos2)) != std::string::npos)//出现次数
						{
							++dbCount_numerator2;
						//	pos2 = pos3+wstrSubRef.size();
						}
						///////////////////N = 3
#ifdef	__EXCLUDE_STOPWORDS
						if ((wstrSubRef3.find(L"/w",0) != std::string::npos) ||
							(wstrSubRef3.find(L"/e",0) != std::string::npos) ||
							(wstrSubRef3.find(L"/y",0) != std::string::npos) ||
							(wstrSubRef3.find(L"/o",0) != std::string::npos) ||
							(wstrSubRef3.find(L"/u",0) != std::string::npos) )

#else
						if (wstrSubRef3.find(L"/w",0) != std::string::npos)
#endif
						{
							pos = pos1+1;
							nN1 = nGram1;
							nN2 = nGram2;
							nN3 = nGram3;
							nN4 = nGram4;
							continue;
						}
						++dbCount_Denominator3;
						pos3 = pos2 = 0;
						if((pos3 = wstrCandidate.find(wstrSubRef3,pos2)) != std::string::npos)//出现次数
						{
							++dbCount_numerator3;
						//	pos2 = pos3+wstrSubRef.size();
						}	
						///////////////////N = 4;
#ifdef	__EXCLUDE_STOPWORDS
						if ((wstrSubRef4.find(L"/w",0) != std::string::npos) ||
							(wstrSubRef4.find(L"/e",0) != std::string::npos) ||
							(wstrSubRef4.find(L"/y",0) != std::string::npos) ||
							(wstrSubRef4.find(L"/o",0) != std::string::npos) ||
							(wstrSubRef4.find(L"/u",0) != std::string::npos) )

#else
						if (wstrSubRef4.find(L"/w",0) != std::string::npos)
#endif
						{
							pos = pos1+1;
							nN1 = nGram1;
							nN2 = nGram2;
							nN3 = nGram3;
							nN4 = nGram4;
							continue;
						}
						++dbCount_Denominator4;
						pos3 = pos2 = 0;
						if((pos3 = wstrCandidate.find(wstrSubRef4,pos2)) != std::string::npos)//出现次数
						{
							++dbCount_numerator4;
						//	pos2 = pos3+wstrSubRef.size();
						}	
						///////////

						pos = pos1+1;
						nN1 = nGram1;
						nN2 = nGram2;
						nN3 = nGram3;
						nN4 = nGram4;
					}
					if (dbCount_Denominator1 > 0)
					{
						t_dbScore = dbCount_numerator1/dbCount_Denominator1;
						if (t_dbScore > dbScore1)
							dbScore1 = t_dbScore;
						dbCount_numerator1 = 0;
						dbCount_Denominator1 = 0;
					}

					if (dbCount_Denominator2 > 0)
					{
						t_dbScore = dbCount_numerator2/dbCount_Denominator2;
						
						if (t_dbScore > dbScore2)
							dbScore2 = t_dbScore;
						dbCount_numerator2 = 0;
						dbCount_Denominator2 = 0;

					}
					if (dbCount_Denominator3 > 0)
					{
						t_dbScore = dbCount_numerator3/dbCount_Denominator3;
						if (t_dbScore > dbScore3)
							dbScore3 = t_dbScore;
						dbCount_numerator3 = 0;

						dbCount_Denominator3 = 0;

					}
					if (dbCount_Denominator4 > 0)
					{
						t_dbScore = dbCount_numerator4/dbCount_Denominator4;
						if (t_dbScore > dbScore4)
							dbScore4 = t_dbScore;
						dbCount_numerator4 = 0;
						dbCount_Denominator4 = 0;

					}


				}
			}
		}

	}
	else
	{//字为单位
		if (vecReference.size() > 0)
		{
			for(int nRIndex=0; nRIndex < (int)vecReference.size(); nRIndex++)
			{
				std::wstring wstrReference = vecReference.at(nRIndex);
				if ((int)wstrReference.size() >= nGram1)
				{
					dbCount_Denominator1 = (wstrReference.size()-nGram1+1);
					dbCount_Denominator2 = (wstrReference.size()-nGram2+1);
					dbCount_Denominator3 = (wstrReference.size()-nGram3+1);
					dbCount_Denominator4 = (wstrReference.size()-nGram4+1);
					pos = pos1 = 0;
					for(int nCIndex=0; nCIndex < (int)wstrReference.size()-nGram1+1; nCIndex++)
					{
						wstrSubRef1 = wstrReference.substr(nCIndex,nGram1);
						if (IsPunctuation(wstrSubRef1))
						{
					//		--dbCount_Denominator1;
					//		--dbCount_Denominator2;
					//		--dbCount_Denominator3;
					//		--dbCount_Denominator4;
							continue;
						}
						if((pos1 = wstrCandidate.find(wstrSubRef1,pos)) !=std::string::npos)//出现最大次数
						{
							++dbCount_numerator1;
						}
						if ((nCIndex+nGram2) <= (int)wstrReference.size())
						{
						wstrSubRef2 = wstrReference.substr(nCIndex,nGram2);
						if (IsPunctuation(wstrSubRef2))
						{
							
						//	--dbCount_Denominator2;
						//	--dbCount_Denominator3;
						//	--dbCount_Denominator4;
							continue;
						}
						if((pos1 = wstrCandidate.find(wstrSubRef2,pos)) !=std::string::npos)//出现最大次数
						{
							++dbCount_numerator2;
						}
						}
						else
						{
							TRACE("N2 is end\n");
						}

						if ((nCIndex+nGram3) <= (int)wstrReference.size())
						{
							wstrSubRef3 = wstrReference.substr(nCIndex,nGram3);
							if (IsPunctuation(wstrSubRef3))
							{
								
						//		--dbCount_Denominator3;
						//		--dbCount_Denominator4;
								continue;
							}
							if((pos1 = wstrCandidate.find(wstrSubRef3,pos)) !=std::string::npos)//出现最大次数
							{
								++dbCount_numerator3;
					
							}
						}
						else
						{
							TRACE("N3 is end\n");
						}

						if ((nCIndex+nGram4) <= (int)wstrReference.size())
						{
							wstrSubRef4 = wstrReference.substr(nCIndex,nGram4);
							if (IsPunctuation(wstrSubRef4))
							{
								
							//	--dbCount_Denominator4;
								continue;
							}
							if((pos1 = wstrCandidate.find(wstrSubRef4,pos)) !=std::string::npos)//出现最大次数
							{
								++dbCount_numerator4;
					
							}
						}
						else
						{
							TRACE("N4 is end\n");
						}

					}

					if (dbCount_Denominator1 > 0)
					{
						t_dbScore = dbCount_numerator1/dbCount_Denominator1;
						if (t_dbScore > dbScore1)
							dbScore1 = t_dbScore;
						dbCount_numerator1 = 0;
						dbCount_Denominator1 = 0;
					}
					if (dbCount_Denominator2 > 0)
					{
						t_dbScore = dbCount_numerator2/dbCount_Denominator2;
						if (t_dbScore > dbScore2)
							dbScore2 = t_dbScore;
						dbCount_numerator2 = 0;
						dbCount_Denominator2 = 0;
					}
					if (dbCount_Denominator3 > 0)
					{
						t_dbScore = dbCount_numerator3/dbCount_Denominator3;
						if (t_dbScore > dbScore3)
							dbScore3 = t_dbScore;
						dbCount_numerator3 = 0;
						dbCount_Denominator3 = 0;
					}

					if (dbCount_Denominator4 > 0)
					{
						t_dbScore = dbCount_numerator4/dbCount_Denominator4;
						if (t_dbScore > dbScore4)
							dbScore4 = t_dbScore;
						dbCount_numerator4 = 0;
						dbCount_Denominator4 = 0;
					}


				}

			}
		}

	}


	m_dbScoreN1 = dbScore1;
	m_dbScoreN2 = dbScore2;
	a1 = 1.0/3.0;
	a2 = 2.0/3.0;
	a3 = 0;
	m_dbScoreC2 = a1*dbScore1 + a2*dbScore2 + a3*dbScore3;
	m_dbScoreN3 = dbScore3;
	a1 = 1.0/6.0; a2 = 2.0/6.0; a3 = 3.0/6.0;
	m_dbScoreC3 = a1*dbScore1 + a2*dbScore2 + a3*dbScore3;
	m_dbScoreN4 = dbScore4;
	 
	
	double Wn = 1.0/2.0;
	double t_dbScoreN1(m_dbScoreN1),t_dbScoreN2(m_dbScoreN2),t_dbScoreN3(m_dbScoreN3),t_dbScoreN4(m_dbScoreN4);
	if (t_dbScoreN1 == 0)
		t_dbScoreN1 = 0.0;
	if (t_dbScoreN2 == 0.0)
		t_dbScoreN2 = 0.0;
	if (t_dbScoreN3 == 0.0)
		t_dbScoreN3 = 0.0;
	if (t_dbScoreN4 == 0.0)
		t_dbScoreN4 = 0.0;

	m_db2gram = exp(Wn*(log(t_dbScoreN1) + log(t_dbScoreN2) ));
	Wn = 1.0/3.0;
	m_db3gram =  exp(Wn*(log(t_dbScoreN1) + log(t_dbScoreN2) + log(t_dbScoreN3)));
	Wn = 1.0/4.0;
	m_db4gram  = exp(Wn*(log(t_dbScoreN1) + log(t_dbScoreN2) + log(t_dbScoreN3) + log(t_dbScoreN4)));

	if(bSeg)
	{
		m_dbScoreSN1 = m_dbScoreN1;
	}
	else
	{
		m_dbScoreON1 = m_dbScoreN1;
	}

}



double CRouge::Rouge_L(std::wstring wstrCandidate,std::vector<std::wstring> vecReference,BOOL bSeg)
{
	double dbScore(0.0),t_dbScore(0.0);
	double dbCount_Denominator(0.0),dbCount_numerator(0.0);//分母，分子
	m_dbScoreL = 0;
	m_dbScoreNPL = 0;
	m_dbScoreW = 0;
	size_t pos(0), pos1(0),pos2(0),pos3(0);
	if (bSeg)//词为单位
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

				if (m_dbTScoreNPL > m_dbScoreNPL)
					m_dbScoreNPL = m_dbTScoreNPL;
				if (m_dbTScoreW > m_dbScoreW)
					m_dbScoreW = m_dbTScoreW;
				m_dbTScoreW = 0;
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

				if (m_dbTScoreNPL > m_dbScoreNPL)
					m_dbScoreNPL = m_dbTScoreNPL;
				if (m_dbTScoreW > m_dbScoreW)
					m_dbScoreW = m_dbTScoreW;
				m_dbTScoreW = 0;
				
			}
		}
	}
	m_dbScoreL = dbScore;
	return dbScore;
}

double CRouge::NormalizedPairwiseLCS(std::wstring wstrCandidate,std::vector<std::wstring> vecReference,BOOL bSeg)
{
	double dbScore(0.0),t_dbScore(0.0);
	double dbCount_Denominator(0.0),dbCount_numerator(0.0);//分母，分子
	
	size_t pos(0), pos1(0),pos2(0),pos3(0);
	if (bSeg)//词为单位
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

double CRouge::Rouge_W(std::wstring wstrCandidate,std::vector<std::wstring> vecReference,BOOL bSeg)
{
	double dbScore(0.0),t_dbScore(0.0);
	double dbCount_Denominator(0.0),dbCount_numerator(0.0);//分母，分子
	
	size_t pos(0), pos1(0),pos2(0),pos3(0);
	if (bSeg)//词为单位
	{
		if (vecReference.size() > 0)
		{
			for(int nRIndex=0; nRIndex < (int)vecReference.size(); nRIndex++)
			{
				std::wstring wstrReference = vecReference.at(nRIndex);

			//	t_dbScore = ComputeFlcs_Sentence(wstrCandidate,wstrReference,bSeg);
				t_dbScore = ComputeFwlcs(wstrCandidate,wstrReference,bSeg);
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
				t_dbScore = ComputeFwlcs(wstrCandidate,wstrReference,bSeg);
				
				if (t_dbScore > dbScore)
					dbScore = t_dbScore;

				
			}
		}
	}

	return dbScore;
}


double CRouge::Rouge_S(std::wstring wstrCandidate,std::vector<std::wstring> vecReference,BOOL bSeg)
{
	double dbScore(0.0),t_dbScore(0.0);
	double dbCount_Denominator(0.0),dbCount_numerator(0.0);//分母，分子
	 m_dbScoreS = 0;
	 m_dbScoreSU = 0;
	size_t pos(0), pos1(0),pos2(0),pos3(0);
	if (bSeg)//词为单位
	{
		if (vecReference.size() > 0)
		{
			for(int nRIndex=0; nRIndex < (int)vecReference.size(); nRIndex++)
			{
				std::wstring wstrReference = vecReference.at(nRIndex);

			
				t_dbScore = ComputeFskip2(wstrCandidate,wstrReference,bSeg);
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
	
				t_dbScore = ComputeFskip2(wstrCandidate,wstrReference,bSeg);
				
				if (t_dbScore > dbScore)
					dbScore = t_dbScore;
				
			}
			
		}
	}
	m_dbScoreS = dbScore;
	m_dbScoreSU = dbScore;
	///////////////
	CString t_Score;
	t_Score.Format("%.04f",dbScore);
	if(t_Score < "0.0001")
	{
		if (bSeg)
			m_dbScoreSU = m_dbScoreSN1;
		else
			m_dbScoreSU = m_dbScoreON1;

	}
	return dbScore;

}
//compute LCS-base F-measure
//整块计算
double  CRouge::ComputeFlcs_Sentence(std::wstring &wstrCandidate,
	std::wstring &wstrReference,
	BOOL bSeg)
{
	double dbFlcs(0.0);
	double nLCS(0),nR(0),nC(0);
	GetLCS(nLCS,nR,nC,wstrCandidate,wstrReference,bSeg);
	double Rlcs = static_cast<double>(nLCS)/static_cast<double>(nR);
	double Plcs = static_cast<double>(nLCS)/static_cast<double>(nC);
	double beta = Plcs/Rlcs;
	dbFlcs = ((1.0 + beta*beta)*Rlcs*Plcs)/(Rlcs + beta*beta*Plcs);
/*	int **pTable,nrowSize(0),ncolumnSize(0);
	std::vector<std::wstring>  m_vectCandidate,m_vectReference;
	std::wstring wstrTemp(L""),wstrPunctuation(L"，。！（） ");
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
//分摘要计算
//ROUGE-L takes the union LCS score
double CRouge::ComputeFlcs_Summary(std::wstring  &wstrCandidate,std::wstring  &wstrReference,BOOL bSeg)
{
	double dbFlcs(0.0),dbFNPLlcs(0.0),dbFwlcs(0.0);
	double nLCS(0),nR(0),nC(0),nNPLCS(0),nNPLR(0),nNPLC(0),nWLCS(0),nWR(0),nWC(0);
	std::size_t pos(0),pos1(0),pos2(0),pos3(0);
	std::vector<std::wstring> vectUnionLCS;
	if (bSeg)
	{
		std::wstring  wstrCSentence(L""),wstrRSentence(L"");
		double t_nSubLCS(0),t_nLCS(0),t_nR(0),t_nC(0),t_nSubNPL(0),t_nNPL(0),t_NPLR(0),t_NPLC(0),t_nSubWLCS(0),t_nWLCS(0),t_nWR(0),t_nWC(0);
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
			//	std::vector<std::wstring> t_LCS = GetLCS(t_nLCS,t_nR,t_nC,wstrCSentence,wstrRSentence,bSeg);
				std::vector<std::wstring> t_LCS = GetWLLCS(t_nLCS,t_nR,t_nC,t_nWLCS,wstrCSentence,wstrRSentence,bSeg);
				t_nNPL = t_nLCS;
				t_NPLR = t_nWR = t_nR;
				t_NPLC = t_nWC = t_nC;
				
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

				///////////////////NPL

				if (t_nSubNPL < t_nNPL)
					t_nSubNPL = t_nNPL;
				if (pos == 0)
					nNPLC += t_NPLC;
				/////////////////////
				/////////////////WLCS
			//	GetWLCS(t_nWLCS,t_nWR,t_nWC,wstrCSentence,wstrRSentence,bSeg);
				if (t_nSubWLCS < t_nWLCS)
					t_nSubWLCS = t_nWLCS;
				if (pos == 0)
					nWC += t_nWC;
				/////////////////////end
			//	double t_nLCS1,t_nR1,t_nC1,t_nWLCS1;
			//	std::vector<std::wstring> t_LCS1 = GetWLLCS(t_nLCS1,t_nR1,t_nC1,t_nWLCS1,wstrCSentence,wstrRSentence,bSeg);
				/////////////////////////////
				pos2 = pos3 + 4;
			}
			t_nSubLCS = vectUnionLCS.size();
			nR += t_nR;
			nLCS += t_nSubLCS;//(t_nSubLCS/t_nR);是否要除每句词组总数?
			t_nSubLCS = 0,t_nLCS = 0,t_nR = 0,t_nC = 0;

			///////////NPL
			nNPLR += t_NPLR;
			nNPLCS += t_nSubNPL;
			t_nSubNPL = 0,t_nLCS = 0,t_NPLR = 0,t_NPLC = 0;
			////////////////////////WLCS
			nWR += t_nWR;
			nWLCS += t_nSubWLCS;
			t_nSubWLCS = 0,t_nWLCS = 0,t_nWR = 0,t_nWC = 0;

			///////////////end


			pos = pos1 + 4;

		}

	}
	else
	{
		std::wstring  wstrCSentence(L""),wstrRSentence(L"");
		double t_nSubLCS(0),t_nLCS(0),t_nR(0),t_nC(0),t_nSubNPL(0),t_nNPL(0),t_NPLR(0),t_NPLC(0),t_nSubWLCS(0),t_nWLCS(0),t_nWR(0),t_nWC(0);
		while(((pos1 = wstrReference.find(L"。",pos)) != std::string::npos) || 
			((pos1 = wstrReference.find(L"？",pos)) != std::string::npos) ||
			((pos1 = wstrReference.find(L"！",pos)) != std::string::npos) ||
			((pos1 = wstrReference.find(L"；",pos)) != std::string::npos) ||
			((pos1 = wstrReference.find(L".",pos)) != std::string::npos)) 

		{
			wstrRSentence = wstrReference.substr(pos, pos1-pos);
			pos2 = pos3 = 0;
			while(((pos3 = wstrCandidate.find(L"。",pos2)) != std::string::npos) || 
			((pos3 = wstrCandidate.find(L"？",pos2)) != std::string::npos) ||
			((pos3 = wstrCandidate.find(L"！",pos2)) != std::string::npos) ||
			((pos3 = wstrCandidate.find(L"；",pos2)) != std::string::npos) ||
			((pos3 = wstrCandidate.find(L".",pos2)) != std::string::npos))
			{
				wstrCSentence = wstrCandidate.substr(pos2,pos3-pos2);
				t_nLCS = 0,t_nR = 0,t_nC = 0;
			//	GetLCS(t_nLCS,t_nR,t_nC,wstrCSentence,wstrRSentence,bSeg);

				std::vector<std::wstring> t_LCS1 = GetWLLCS(t_nLCS,t_nR,t_nC,t_nWLCS,wstrCSentence,wstrRSentence,bSeg);
				t_nNPL = t_nLCS;
				t_NPLR =t_nWR =  t_nR;
				t_NPLC = t_nWC = t_nC;
				if (t_nSubLCS < t_nLCS)
					t_nSubLCS = t_nLCS;
				if (pos == 0)
					nC += t_nC;

				////////////////////NPL
				
				if (t_nSubNPL < t_nNPL)
					t_nSubNPL = t_nNPL;
				if (pos == 0)
					nNPLC += t_NPLC;

				/////////////////end
				////////////WLCS
		//		GetWLCS(t_nWLCS,t_nWR,t_nWC,wstrCSentence,wstrRSentence,bSeg);
				if (t_nSubWLCS < t_nWLCS)
					t_nSubWLCS = t_nWLCS;
				if (pos == 0)
					nWC += t_nWC;

				/////////////////			
			
				pos2 = pos3 + 1;
			}
			nR += t_nR;
			nLCS += t_nSubLCS;
			t_nSubLCS = 0,t_nLCS = 0,t_nR = 0,t_nC = 0;

			///////////////////NPL
			nNPLR += t_NPLR;
			nNPLCS += t_nSubNPL;
			t_nSubNPL = 0,t_nLCS = 0,t_NPLR = 0,t_NPLC = 0;
			//////////////////////WLCS
			nWR += t_nWR;
			nWLCS += t_nSubWLCS;
			t_nSubWLCS = 0,t_nWLCS = 0,t_nWR = 0,t_nWC = 0;
			//////////////////
			pos = pos1 + 1;

		}

	}

	double Rlcs = static_cast<double>(nLCS)/static_cast<double>(nR);
	double Plcs = static_cast<double>(nLCS)/static_cast<double>(nC);
	double beta = Plcs/Rlcs;
	dbFlcs = ((1.0 + beta*beta)*Rlcs*Plcs)/(Rlcs + beta*beta*Plcs);
	m_dbScoreL = dbFlcs;
	//////////NPL
	double RNPLlcs = static_cast<double>(nNPLCS)/static_cast<double>(nNPLR);
	double PNPLlcs = static_cast<double>(nNPLCS)/static_cast<double>(nNPLC);
	double betaNPL = 1.0;//Plcs/Rlcs;
	m_dbTScoreNPL = ((1.0 + betaNPL*betaNPL)*RNPLlcs*PNPLlcs)/(RNPLlcs + betaNPL*betaNPL*PNPLlcs);
	
	m_dbScoreNPL = dbFNPLlcs;
	////////////////////WLCS
	double Rwlcs = inversef(static_cast<double>(nWLCS)/static_cast<double>(f(nWR)));
	double Pwlcs = inversef(static_cast<double>(nWLCS)/static_cast<double>(f(nWC)));
	double betaWLCS = 1.0;//Plcs/Rlcs;
	dbFwlcs = ((1.0 + betaWLCS*betaWLCS)*Rwlcs*Pwlcs)/(Rwlcs + betaWLCS*betaWLCS*Pwlcs);
	m_dbTScoreW = dbFwlcs;
	////////////////

	
	return dbFlcs;

}

////Normalized Pairwise LCS take the bestLCS score
double CRouge::ComputeFlcs_NormalizedPairwiseLCS(std::wstring  &wstrCandidate,std::wstring  &wstrReference,BOOL bSeg)
{
	double dbFlcs(0.0);
	double nLCS(0),nR(0),nC(0);
	std::size_t pos(0),pos1(0),pos2(0),pos3(0);
	
	if (bSeg)
	{
		std::wstring  wstrCSentence(L""),wstrRSentence(L"");
		double t_nSubLCS(0),t_nLCS(0),t_nR(0),t_nC(0);
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
		double t_nSubLCS(0),t_nLCS(0),t_nR(0),t_nC(0);
		while(((pos1 = wstrReference.find(L"。",pos)) != std::string::npos) || 
			((pos1 = wstrReference.find(L"？",pos)) != std::string::npos) ||
			((pos1 = wstrReference.find(L"！",pos)) != std::string::npos) ||
			((pos1 = wstrReference.find(L"；",pos)) != std::string::npos))
		{
			wstrRSentence = wstrReference.substr(pos, pos1-pos);
			pos2 = pos3 = 0;
			while(((pos3 = wstrCandidate.find(L"。",pos2)) != std::string::npos) || 
			((pos3 = wstrCandidate.find(L"？",pos2)) != std::string::npos) ||
			((pos3 = wstrCandidate.find(L"！",pos2)) != std::string::npos) ||
			((pos3 = wstrCandidate.find(L"；",pos2)) != std::string::npos))
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


double CRouge::ComputeFwlcs(std::wstring  &wstrCandidate,std::wstring  &wstrReference,BOOL bSeg)
{
	double dbFwlcs(0.0);
	long nWLCS(0),nR(0),nC(0);
	std::size_t pos(0),pos1(0),pos2(0),pos3(0);
	
	if (bSeg)
	{
		std::wstring  wstrCSentence(L""),wstrRSentence(L"");
		double t_nSubWLCS(0),t_nWLCS(0),t_nR(0),t_nC(0);
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
				t_nWLCS = 0,t_nR = 0,t_nC = 0;
				GetWLCS(t_nWLCS,t_nR,t_nC,wstrCSentence,wstrRSentence,bSeg);
				if (t_nSubWLCS < t_nWLCS)
					t_nSubWLCS = t_nWLCS;
				if (pos == 0)
					nC += t_nC;
				
				pos2 = pos3 + 4;
			}
			nR += t_nR;
			nWLCS += t_nSubWLCS;
			t_nSubWLCS = 0,t_nWLCS = 0,t_nR = 0,t_nC = 0;
			pos = pos1 + 4;

		}

	}
	else
	{
		std::wstring  wstrCSentence(L""),wstrRSentence(L"");
		double t_nSubWLCS(0),t_nWLCS(0),t_nR(0),t_nC(0);
		while(((pos1 = wstrReference.find(L"。",pos)) != std::string::npos) || 
			((pos1 = wstrReference.find(L"？",pos)) != std::string::npos) ||
			((pos1 = wstrReference.find(L"！",pos)) != std::string::npos) ||
			((pos1 = wstrReference.find(L"；",pos)) != std::string::npos))
		{
			wstrRSentence = wstrReference.substr(pos, pos1-pos);
			pos2 = pos3 = 0;
			while(((pos3 = wstrCandidate.find(L"。",pos2)) != std::string::npos) || 
			((pos3 = wstrCandidate.find(L"？",pos2)) != std::string::npos) ||
			((pos3 = wstrCandidate.find(L"！",pos2)) != std::string::npos) ||
			((pos3 = wstrCandidate.find(L"；",pos2)) != std::string::npos))
			{
				wstrCSentence = wstrCandidate.substr(pos2,pos3-pos2);
				t_nWLCS = 0,t_nR = 0,t_nC = 0;
				GetWLCS(t_nWLCS,t_nR,t_nC,wstrCSentence,wstrRSentence,bSeg);
				if (t_nSubWLCS < t_nWLCS)
					t_nSubWLCS = t_nWLCS;
				if (pos == 0)
					nC += t_nC;
				
				pos2 = pos3 + 1;
			}
			nR += t_nR;
			nWLCS += t_nSubWLCS;
			t_nSubWLCS = 0,t_nWLCS = 0,t_nR = 0,t_nC = 0;
			pos = pos1 + 1;

		}

	}

	double Rwlcs = inversef(static_cast<double>(nWLCS)/static_cast<double>(f(nR)));
	double Pwlcs = inversef(static_cast<double>(nWLCS)/static_cast<double>(f(nC)));
	double beta = 1.0;//Plcs/Rlcs;
	dbFwlcs = ((1.0 + beta*beta)*Rwlcs*Pwlcs)/(Rwlcs + beta*beta*Pwlcs);
	return dbFwlcs;

}

double CRouge::ComputeFskip2(std::wstring  &wstrCandidate,std::wstring  &wstrReference,BOOL bSeg)
{
	double dbFskip2(0.0);

	int nSkip2Num(0),nR(0),nC(0);
	std::size_t pos(0),pos1(0),pos2(0),pos3(0);
	
	if (bSeg)
	{
		std::wstring  wstrCSentence(L""),wstrRSentence(L"");
		long t_nSubSKIP2(0),t_nSKIP2(0),t_nR(0),t_nC(0);
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
				t_nSKIP2 = 0,t_nR = 0,t_nC = 0;
				GetSKIP2(t_nSKIP2,t_nR,t_nC,wstrCSentence,wstrRSentence,bSeg);
				if (t_nSubSKIP2 < t_nSKIP2)
					t_nSubSKIP2 = t_nSKIP2;
				if (pos == 0)
					nC += t_nC;
				
				pos2 = pos3 + 4;
			}
			nR += t_nR;
			nSkip2Num += t_nSubSKIP2;
			t_nSubSKIP2 = 0,t_nSKIP2 = 0,t_nR = 0,t_nC = 0;
			pos = pos1 + 4;

		}

	}
	else
	{
		std::wstring  wstrCSentence(L""),wstrRSentence(L"");
		long t_nSubSKIP2(0),t_nSKIP2(0),t_nR(0),t_nC(0);
		while(((pos1 = wstrReference.find(L"。",pos)) != std::string::npos) || 
			((pos1 = wstrReference.find(L"？",pos)) != std::string::npos) ||
			((pos1 = wstrReference.find(L"！",pos)) != std::string::npos) ||
			((pos1 = wstrReference.find(L"；",pos)) != std::string::npos))
		{
			wstrRSentence = wstrReference.substr(pos, pos1-pos);
			pos2 = pos3 = 0;
			while(((pos3 = wstrCandidate.find(L"。",pos2)) != std::string::npos) || 
			((pos3 = wstrCandidate.find(L"？",pos2)) != std::string::npos) ||
			((pos3 = wstrCandidate.find(L"！",pos2)) != std::string::npos) ||
			((pos3 = wstrCandidate.find(L"；",pos2)) != std::string::npos))
			{
				wstrCSentence = wstrCandidate.substr(pos2,pos3-pos2);
				t_nSKIP2 = 0,t_nR = 0,t_nC = 0;
				GetSKIP2(t_nSKIP2,t_nR,t_nC,wstrCSentence,wstrRSentence,bSeg);
				if (t_nSubSKIP2 < t_nSKIP2)
					t_nSubSKIP2 = t_nSKIP2;
				if (pos == 0)
					nC += t_nC;
				
				pos2 = pos3 + 1;
			}
			nR += t_nR;
			nSkip2Num += t_nSubSKIP2;
			t_nSubSKIP2 = 0,t_nSKIP2 = 0,t_nR = 0,t_nC = 0;
			pos = pos1 + 1;

		}

	}

//	double Rlcs = static_cast<double>(nLCS)/static_cast<double>(nR);
//	double Plcs = static_cast<double>(nLCS)/static_cast<double>(nC);
//	double beta = 1.0;//Plcs/Rlcs;
//	dbFskip2 = ((1.0 + beta*beta)*Rlcs*Plcs)/(Rlcs + beta*beta*Plcs);


	double Rskip2(0),Pskip2(0),beta = 1.0;
	if (nR > 0)
	Rskip2 = static_cast<double>(nSkip2Num)/static_cast<double>(nR);
	if (nC > 0)
		Pskip2 = static_cast<double>(nSkip2Num)/static_cast<double>(nC);
	if (Rskip2 > 0 || Pskip2 > 0)
	dbFskip2 = ((1+beta*beta)*Rskip2*Pskip2)/(Rskip2+beta*beta*Pskip2);
	
	///////////////
	return dbFskip2;
}

std::vector<std::wstring> CRouge::GetLCS(double &lcs,double &mR,double &nC,std::wstring  &wstrCandidate,std::wstring  &wstrReference,BOOL bSeg)
{
	double dbFlcs(0.0);
	int nrowSize(0),ncolumnSize(0); //**pTable,**prev,
	std::vector<std::wstring>  m_vectCandidate,m_vectReference,m_vectLCS;
	std::wstring wstrTemp(L""),wstrPunctuation(L"，。！（） ");
	if (bSeg)///segment
	{
		std::size_t pos1(0),pos(0);


		while((pos1 = wstrCandidate.find(L" ",pos)) != std::string::npos)
		{

#ifdef	__EXCLUDE_STOPWORDS
			if ((wstrCandidate.substr(pos,pos1-pos).find(L"/w",0) != std::string::npos) ||
				(wstrCandidate.substr(pos,pos1-pos).find(L"/e",0) != std::string::npos) ||
				(wstrCandidate.substr(pos,pos1-pos).find(L"/y",0) != std::string::npos) ||
				(wstrCandidate.substr(pos,pos1-pos).find(L"/o",0) != std::string::npos) ||
				(wstrCandidate.substr(pos,pos1-pos).find(L"/u",0) != std::string::npos) )

#else
			size_t t_pos = wstrCandidate.substr(pos,pos1-pos).find(L"/w",0);
			if (t_pos != std::string::npos)
#endif
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
#ifdef	__EXCLUDE_STOPWORDS
			if ((wstrReference.substr(pos,pos1-pos).find(L"/w",0) != std::string::npos) ||
				(wstrReference.substr(pos,pos1-pos).find(L"/e",0) != std::string::npos) ||
				(wstrReference.substr(pos,pos1-pos).find(L"/y",0) != std::string::npos) ||
				(wstrReference.substr(pos,pos1-pos).find(L"/o",0) != std::string::npos) ||
				(wstrReference.substr(pos,pos1-pos).find(L"/u",0) != std::string::npos) )

#else
			size_t t_pos = wstrReference.substr(pos,pos1-pos).find(L"/w",0);
			if (t_pos != std::string::npos)
#endif
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
			if (IsPunctuation(wstrTemp))
				continue;
			m_vectCandidate.push_back(wstrTemp);
			if (ix == 0)
				m_vectCandidate.push_back(wstrTemp);
		}
		wstrTemp = wstrReference.at(0);
		m_vectReference.push_back(wstrTemp);
		for(std::vector<std::wstring>::size_type ir=0; ir != wstrReference.size(); ++ir)
		{
			wstrTemp = wstrReference.at(ir);
			if (wstrPunctuation.find(wstrTemp,0) != std::string::npos || wstrTemp.at(0) == 13)
				continue;
			if (IsPunctuation(wstrTemp))
				continue;
			m_vectReference.push_back(wstrTemp);
			if (ir == 0)
				m_vectReference.push_back(wstrTemp);
		}


	}

	nrowSize = m_vectCandidate.size();
	nC = nrowSize-1;
	ncolumnSize = m_vectReference.size();
	mR = ncolumnSize - 1;
	pTable = new int*[nrowSize];
	pTable[0] = new int[nrowSize*ncolumnSize];
	memset(pTable[0],0,sizeof(int)*nrowSize*ncolumnSize);
	for(int i = 1; i != nrowSize; i++)
	{
		pTable[i] = pTable[i-1]+ncolumnSize;
	}

	
	prev = new int*[nrowSize];//1:left top 2:left 3:top
	prev[0] = new int[nrowSize*ncolumnSize];
	memset(prev[0],0,sizeof(int)*nrowSize*ncolumnSize);
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
			//	TRACE("pTable[%d][%d]=%d \n",nRow,nColumn,pTable[nRow][nColumn]);
			}
			else
			{
				if (pTable[nRow-1][nColumn] > pTable[nRow][nColumn-1])
				{
					pTable[nRow][nColumn] = pTable[nRow-1][nColumn];
					prev[nRow][nColumn] = 3;
				//	TRACE("pTable[%d][%d]=%d \n",nRow,nColumn,pTable[nRow][nColumn]);
				}
				else
				{
					pTable[nRow][nColumn] =pTable[nRow][nColumn-1];
					prev[nRow][nColumn] = 2;
				//	TRACE("pTable[%d][%d]=%d \n",nRow,nColumn,pTable[nRow][nColumn]);
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


void CRouge::GetWLCS(double &wlcs,double &mR,double &nC,std::wstring  &wstrCandidate,std::wstring  &wstrReference,BOOL bSeg)
{
	double dbFlcs(0.0);
	int nrowSize(0),ncolumnSize(0); //**pTable,**prev,
	std::vector<std::wstring>  m_vectCandidate,m_vectReference,m_vectLCS;
	std::wstring wstrTemp(L""),wstrPunctuation(L"，。！（） ");
	if (bSeg)///segment
	{
		std::size_t pos1(0),pos(0);

		while((pos1 = wstrCandidate.find(L" ",pos)) != std::string::npos)
		{
#ifdef	__EXCLUDE_STOPWORDS
			if ((wstrCandidate.substr(pos,pos1-pos).find(L"/w",0) != std::string::npos) ||
				(wstrCandidate.substr(pos,pos1-pos).find(L"/e",0) != std::string::npos) ||
				(wstrCandidate.substr(pos,pos1-pos).find(L"/y",0) != std::string::npos) ||
				(wstrCandidate.substr(pos,pos1-pos).find(L"/o",0) != std::string::npos) ||
				(wstrCandidate.substr(pos,pos1-pos).find(L"/u",0) != std::string::npos) )

#else
			size_t t_pos = wstrCandidate.substr(pos,pos1-pos).find(L"/w",0);

			if (t_pos != std::string::npos)
#endif
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
#ifdef	__EXCLUDE_STOPWORDS
			if ((wstrReference.substr(pos,pos1-pos).find(L"/w",0) != std::string::npos) ||
				(wstrReference.substr(pos,pos1-pos).find(L"/e",0) != std::string::npos) ||
				(wstrReference.substr(pos,pos1-pos).find(L"/y",0) != std::string::npos) ||
				(wstrReference.substr(pos,pos1-pos).find(L"/o",0) != std::string::npos) ||
				(wstrReference.substr(pos,pos1-pos).find(L"/u",0) != std::string::npos) )

#else
			size_t t_pos = wstrReference.substr(pos,pos1-pos).find(L"/w",0);
			if (t_pos != std::string::npos)
#endif
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
			if (IsPunctuation(wstrTemp))
				continue;
			m_vectCandidate.push_back(wstrTemp);
			if (ix == 0)
				m_vectCandidate.push_back(wstrTemp);
		}
		wstrTemp = wstrReference.at(0);
		m_vectReference.push_back(wstrTemp);
		for(std::vector<std::wstring>::size_type ir=0; ir != wstrReference.size(); ++ir)
		{
			wstrTemp = wstrReference.at(ir);
			if (wstrPunctuation.find(wstrTemp,0) != std::string::npos || wstrTemp.at(0) == 13)
				continue;
			if (IsPunctuation(wstrTemp))
				continue;
			m_vectReference.push_back(wstrTemp);
			if (ir == 0)
				m_vectReference.push_back(wstrTemp);

		}


	}

	nrowSize = m_vectCandidate.size();
	nC = nrowSize-1;
	ncolumnSize = m_vectReference.size();
	mR = ncolumnSize - 1;
	pTable = new int*[nrowSize];
	pTable[0] = new int[nrowSize*ncolumnSize];
	memset(pTable[0],0,sizeof(int)*nrowSize*ncolumnSize);
	for(int i = 1; i != nrowSize; i++)
	{
		pTable[i] = pTable[i-1]+ncolumnSize;
	}

	
	pweight = new int*[nrowSize];//1:left top 2:left 3:top
	pweight[0] = new int[nrowSize*ncolumnSize];
	memset(pweight[0],0,sizeof(int)*nrowSize*ncolumnSize);
	for(int i = 1; i != nrowSize; i++)
	{
		pweight[i] = pweight[i-1]+ncolumnSize;
	}
	int nK(0);
	for (int nRow = 1; nRow != nrowSize; ++nRow)
	{
		for (int nColumn = 1; nColumn != ncolumnSize; ++nColumn)
		{
			if (m_vectCandidate.at(nRow) == m_vectReference.at(nColumn))
			{
				nK = pweight[nRow-1][nColumn-1];
				pTable[nRow][nColumn] = pTable[nRow-1][nColumn-1]+f(nK+1)-f(nK);
	//			TRACE("nK = %d,pTable[%d][%d]=%d\n",nK,nRow,nColumn,pTable[nRow][nColumn]);
				std::wstring temp = m_vectCandidate.at(nRow);
				pweight[nRow][nColumn] = nK + 1;
			}
			else
			{
				if (pTable[nRow-1][nColumn] > pTable[nRow][nColumn-1])
				{
					pTable[nRow][nColumn] = pTable[nRow-1][nColumn];
					pweight[nRow][nColumn] = 0;
				}
				else
				{
					pTable[nRow][nColumn] =pTable[nRow][nColumn-1];
					pweight[nRow][nColumn] = 0;
				}
			}
		}
	}
	wlcs = pTable[nrowSize-1][ncolumnSize-1];

//	double Rlcs = static_cast<double>(nLCS)/static_cast<double>(nrowSize);
//	double Plcs = static_cast<double>(nLCS)/static_cast<double>(ncolumnSize);
//	double beta = Plcs/Rlcs;
//	dbFlcs = ((1.0 + beta*beta)*Rlcs*Plcs)/(Rlcs + beta*beta*Plcs);

//	PrintLCS(m_vectCandidate,nrowSize-1,ncolumnSize-1,m_vectLCS);

	if(pTable)
	{
		delete [](pTable[0]);
		delete []pTable;
	//	pTable[0] = NULL;
	//	pTable = NULL;
	}
	if(pweight)
	{
		delete [](pweight[0]);
		delete []pweight;
	//	prev[0] = NULL;
	//	prev = NULL;
	}


}

std::vector<std::wstring> CRouge::GetWLLCS(double &lcs,double &mR,double &nC,double &wlcs,
		std::wstring  &wstrCandidate,std::wstring  &wstrReference,BOOL bSeg)
{

	double dbFwlcs;//dbFlcs(0.0),
	int nrowSize(0),ncolumnSize(0);
	std::vector<std::wstring>  m_vectCandidate,m_vectReference,m_vectLCS;
	std::wstring wstrTemp(L""),wstrPunctuation(L"，。！（） ");
	if (bSeg)///segment
	{
		std::size_t pos1(0),pos(0);

		while((pos1 = wstrCandidate.find(L" ",pos)) != std::string::npos)
		{
#ifdef	__EXCLUDE_STOPWORDS
			if ((wstrCandidate.substr(pos,pos1-pos).find(L"/w",0) != std::string::npos) ||
				(wstrCandidate.substr(pos,pos1-pos).find(L"/e",0) != std::string::npos) ||
				(wstrCandidate.substr(pos,pos1-pos).find(L"/y",0) != std::string::npos) ||
				(wstrCandidate.substr(pos,pos1-pos).find(L"/o",0) != std::string::npos) ||
				(wstrCandidate.substr(pos,pos1-pos).find(L"/u",0) != std::string::npos) )

#else
			size_t t_pos = wstrCandidate.substr(pos,pos1-pos).find(L"/w",0);
			if (t_pos != std::string::npos)
#endif
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
#ifdef	__EXCLUDE_STOPWORDS
			if ((wstrReference.substr(pos,pos1-pos).find(L"/w",0) != std::string::npos) ||
				(wstrReference.substr(pos,pos1-pos).find(L"/e",0) != std::string::npos) ||
				(wstrReference.substr(pos,pos1-pos).find(L"/y",0) != std::string::npos) ||
				(wstrReference.substr(pos,pos1-pos).find(L"/o",0) != std::string::npos) ||
				(wstrReference.substr(pos,pos1-pos).find(L"/u",0) != std::string::npos) )

#else
			size_t t_pos = wstrReference.substr(pos,pos1-pos).find(L"/w",0);
			if (t_pos != std::string::npos)
#endif
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
			if (IsPunctuation(wstrTemp))
				continue;
			m_vectCandidate.push_back(wstrTemp);
		//	if (ix == 0)
		//		m_vectCandidate.push_back(wstrTemp);
		}
		wstrTemp = wstrReference.at(0);
		m_vectReference.push_back(wstrTemp);
		for(std::vector<std::wstring>::size_type ir=0; ir != wstrReference.size(); ++ir)
		{
			wstrTemp = wstrReference.at(ir);
			if (wstrPunctuation.find(wstrTemp,0) != std::string::npos || wstrTemp.at(0) == 13)
				continue;
			if (IsPunctuation(wstrTemp))
				continue;
			m_vectReference.push_back(wstrTemp);
		//	if (ir == 0)
		//		m_vectReference.push_back(wstrTemp);

		}


	}

	nrowSize = m_vectCandidate.size();
	nC = nrowSize-1;
	ncolumnSize = m_vectReference.size();
	mR = ncolumnSize - 1;
	//Rouge-W
	pTable = new int*[nrowSize];
	pTable[0] = new int[nrowSize*ncolumnSize];
	memset(pTable[0],0,sizeof(int)*nrowSize*ncolumnSize);
	for(int i = 1; i != nrowSize; i++)
	{
		pTable[i] = pTable[i-1]+ncolumnSize;
	}

	//Rouge-L
	pTable0 = new int*[nrowSize];
	pTable0[0] = new int[nrowSize*ncolumnSize];
	memset(pTable0[0],0,sizeof(int)*nrowSize*ncolumnSize);
	for(int i = 1; i != nrowSize; i++)
	{
		pTable0[i] = pTable0[i-1]+ncolumnSize;
	}

	prev = new int*[nrowSize];//1:left top 2:left 3:top
	prev[0] = new int[nrowSize*ncolumnSize];
	memset(prev[0],0,nrowSize*ncolumnSize);
	for(int i = 1; i != nrowSize; i++)
	{
		prev[i] = prev[i-1]+ncolumnSize;
	}
	//////////////
	
	pweight = new int*[nrowSize];//1:left top 2:left 3:top
	pweight[0] = new int[nrowSize*ncolumnSize];
	memset(pweight[0],0,sizeof(int)*nrowSize*ncolumnSize);
	for(int i = 1; i != nrowSize; i++)
	{
		pweight[i] = pweight[i-1]+ncolumnSize;
	}
	int nK(0);
	for (int nRow = 1; nRow != nrowSize; ++nRow)
	{
		for (int nColumn = 1; nColumn != ncolumnSize; ++nColumn)
		{
			if (m_vectCandidate.at(nRow) == m_vectReference.at(nColumn))
			{
				nK = pweight[nRow-1][nColumn-1];
				pTable[nRow][nColumn] = pTable[nRow-1][nColumn-1]+f(nK+1)-f(nK);
	//			TRACE("nK = %d,pTable[%d][%d]=%d\n",nK,nRow,nColumn,pTable[nRow][nColumn]);
				std::wstring temp = m_vectCandidate.at(nRow);
				pweight[nRow][nColumn] = nK + 1;

				pTable0[nRow][nColumn] = pTable0[nRow-1][nColumn-1]+1;
			//	TRACE("pTable0[%d][%d]=%d \n",nRow,nColumn,pTable0[nRow][nColumn]);
				temp = m_vectCandidate.at(nRow);
				prev[nRow][nColumn] = 1;
			}
			else
			{
				/////////////WLCS
				if (pTable[nRow-1][nColumn] > pTable[nRow][nColumn-1])
				{
					pTable[nRow][nColumn] = pTable[nRow-1][nColumn];
					pweight[nRow][nColumn] = 0;

					pTable0[nRow][nColumn] = pTable0[nRow-1][nColumn];
					prev[nRow][nColumn] = 3;
			//		TRACE("pTable0[%d][%d]=%d \n",nRow,nColumn,pTable0[nRow][nColumn]);
				}
				else
				{
					pTable[nRow][nColumn] =pTable[nRow][nColumn-1];
					pweight[nRow][nColumn] = 0;

					pTable0[nRow][nColumn] =pTable0[nRow][nColumn-1];
					prev[nRow][nColumn] = 2;
			//		TRACE("pTable0[%d][%d]=%d \n",nRow,nColumn,pTable0[nRow][nColumn]);
				}

				////////////////LCS
			
			}
		}
	}
	wlcs = pTable[nrowSize-1][ncolumnSize-1];
	lcs = pTable0[nrowSize-1][ncolumnSize-1];
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
	if (pTable0)
	{
		delete [](pTable0[0]);
		delete []pTable0;
	}

	if(pweight)
	{
		delete [](pweight[0]);
		delete []pweight;
	//	prev[0] = NULL;
	//	prev = NULL;
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


void CRouge::GetSKIP2(long &skip2,long &mR,long &nC,std::wstring  &wstrCandidate,std::wstring  &wstrReference,BOOL bSeg)
{
	double dbSkip2(0.0);
	int nrowSize(0),ncolumnSize(0),nSkip2Num(0); //**pTable,**prev,
	std::vector<std::wstring>  m_vectCandidate,m_vectReference,m_vectLCS;
	std::wstring wstrTemp(L""),wstrPunctuation(L"，。！（） ");
	if (bSeg)///segment
	{
		std::size_t pos1(0),pos(0);

		while((pos1 = wstrCandidate.find(L" ",pos)) != std::string::npos)
		{
#ifdef	__EXCLUDE_STOPWORDS
			if ((wstrCandidate.substr(pos,pos1-pos).find(L"/w",0) != std::string::npos) ||
				(wstrCandidate.substr(pos,pos1-pos).find(L"/e",0) != std::string::npos) ||
				(wstrCandidate.substr(pos,pos1-pos).find(L"/y",0) != std::string::npos) ||
				(wstrCandidate.substr(pos,pos1-pos).find(L"/o",0) != std::string::npos) ||
				(wstrCandidate.substr(pos,pos1-pos).find(L"/u",0) != std::string::npos) )

#else
			size_t t_pos = wstrCandidate.substr(pos,pos1-pos).find(L"/w",0);
			if (t_pos != std::string::npos)
#endif
			{
				pos = pos1+1;

				continue;
			}
			wstrTemp = wstrCandidate.substr(pos,pos1-pos);

			m_vectCandidate.push_back(wstrTemp);
		/*	if (pos == 0)
			{
				m_vectCandidate.push_back(wstrTemp);
			}*/
			pos = pos1+1;
		}

		pos1 = 0; 
		pos = 0;

		while((pos1 = wstrReference.find(L" ",pos)) != std::string::npos)
		{
#ifdef	__EXCLUDE_STOPWORDS
			if ((wstrReference.substr(pos,pos1-pos).find(L"/w",0) != std::string::npos) ||
				(wstrReference.substr(pos,pos1-pos).find(L"/e",0) != std::string::npos) ||
				(wstrReference.substr(pos,pos1-pos).find(L"/y",0) != std::string::npos) ||
				(wstrReference.substr(pos,pos1-pos).find(L"/o",0) != std::string::npos) ||
				(wstrReference.substr(pos,pos1-pos).find(L"/u",0) != std::string::npos) )

#else
			size_t t_pos = wstrReference.substr(pos,pos1-pos).find(L"/w",0);
			if (t_pos != std::string::npos)
#endif
			{
				pos = pos1+1;

				continue;
			}
			wstrTemp = wstrReference.substr(pos,pos1-pos);

			m_vectReference.push_back(wstrTemp);
			/*if (pos == 0)
			{
				m_vectReference.push_back(wstrTemp);
			}*/
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
			if (IsPunctuation(wstrTemp))
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
			if (IsPunctuation(wstrTemp))
				continue;
			m_vectReference.push_back(wstrTemp);
		}


	}

	nrowSize = m_vectCandidate.size();
	if (nrowSize > 3)
		nC = (nrowSize  - 3)*3 + 2 + 1;
	else if (nrowSize > 2)
		nC = 3;
	else if (nrowSize > 1)
		nC = 1;
	else 
		nC = 0;
	ncolumnSize = m_vectReference.size();

	if (ncolumnSize > 3)
		mR =  (ncolumnSize-3)*3 + 2 + 1;
	else if (ncolumnSize > 2)
		mR = 3;
	else if (ncolumnSize > 1)
		mR = 1;
	else
		mR = 0;
	std::vector<std::wstring>  m_vectCandidateSkip2,m_vectReferenceSkip2;
	std::wstring wstrPhrase2;
	int nRmainderLenght = 0;
	int nIndex;
	for (nIndex = 0; nIndex != nrowSize; ++nIndex)
	{
		nRmainderLenght = nrowSize - nIndex -1;
		if (nRmainderLenght == 0)
			break;
		else if (nRmainderLenght > 2)
		{
			nRmainderLenght = 3;
		}

		else if (nRmainderLenght > 1)
			nRmainderLenght = 2;
		else if (nRmainderLenght > 0)
			nRmainderLenght = 1;
		for (int nNext = 1; nNext != (nRmainderLenght+1); ++nNext)
		{
			wstrPhrase2 = m_vectCandidate[nIndex]+m_vectCandidate[nIndex + nNext];
			m_vectCandidateSkip2.push_back(wstrPhrase2);
		}
		
	}

	for (nIndex = 0; nIndex != ncolumnSize; ++nIndex)
	{
		nRmainderLenght = ncolumnSize - nIndex -1;
		if (nRmainderLenght == 0)
			break;
		else if (nRmainderLenght > 2)
		{
			nRmainderLenght = 3;
		}
		else if (nRmainderLenght > 1)
			nRmainderLenght = 2;
		else if (nRmainderLenght > 0)
			nRmainderLenght = 1;
		for (int nNext = 1; nNext != (nRmainderLenght+1); ++nNext)
		{
			wstrPhrase2 = m_vectReference[nIndex]+m_vectReference[nIndex + nNext];
			std::vector<std::wstring>::iterator iter = std::find(m_vectCandidateSkip2.begin(), m_vectCandidateSkip2.end(),wstrPhrase2);
			if ( iter != m_vectCandidateSkip2.end())
				++nSkip2Num;
		}
	}

	skip2 = nSkip2Num;
//	double Rskip2(0),Pskip2(0),beta = 1.0;
//	if (mR > 0)
//	Rskip2 = static_cast<double>(nSkip2Num)/static_cast<double>(mR);
//	if (nC > 0)
	//	Pskip2 = static_cast<double>(nSkip2Num)/static_cast<double>(nC);
	//double Fskip2 = ((1+beta*beta)*Rskip2*Pskip2)/(Rskip2+beta*beta*Pskip2);

	

}

bool  CRouge::IsPunctuation(std::wstring  ch)
{
	bool bRet(false);
	if ((ch.find(L",") != std::wstring::npos) //|| (ch.find(L".") != std::wstring::npos) 
		|| (ch.find(L"!") != std::wstring::npos) || (ch.find(L"(") != std::wstring::npos)
		|| (ch.find( L")")!= std::wstring::npos) || (ch.find( L"?") != std::wstring::npos)
		|| (ch.find( L"。") != std::wstring::npos) || (ch.find( L"，") != std::wstring::npos)
		|| (ch.find( L"！") != std::wstring::npos) || (ch.find( L"？") != std::wstring::npos)
		|| (ch.find( L"（")!= std::wstring::npos) || (ch.find( L"）") != std::wstring::npos))
	{
		bRet = true;
	}
	return bRet;
}