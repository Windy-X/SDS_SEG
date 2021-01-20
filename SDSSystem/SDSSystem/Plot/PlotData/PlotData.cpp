#include "PlotData.h"
#include <QDebug>
#include "Config.h"
#include <QtMath>

//const int C_PAGE_DATA_LENGTH_1 = 60 * 125;
CPlotData::CPlotData()
	:m_iPixWidth(0)
	,m_fPreScale(1.0f)
	,m_fNextScale(1.0f)
	,m_iReseverdWidth(0)
	,m_fCurRatio(1.0f)
	,m_iTimeWidth(0)
	,m_iCurIndex(0)
{
	Clear();
}
CPlotData::~CPlotData()
{
	Clear();
}
void CPlotData::Clear()
{
	m_vctRawData.clear();
	m_vctCurData.clear();
	//m_vctCurEvn.clear();
	//m_vctNextEvn.clear();
}
void CPlotData::Init(const int iPixWidth)
{
	Clear();
	//���û�ͼ�����ؿ��
	m_iPixWidth = iPixWidth;
	//��ȡԤ�����ڿ��
	m_iReseverdWidth = CConfig::Instance()->GetReservedWidth();
	m_fPreScale = double(m_iPixWidth - m_iReseverdWidth) / m_iPixWidth;
	m_fNextScale = double(m_iPixWidth) / (m_iPixWidth - m_iReseverdWidth);
	//����ǰ������ add <2020/10/09> xiongz
	m_iStep = m_iReseverdWidth;

	//debug���Զ����������ļ��е�ֵ����ɾ���޸� debug <2020/9/29>xiongz
	m_iTimeWidth = CConfig::Instance()->GetPageDataLen(0);
	m_fCurRatio = double(m_iTimeWidth) / m_iPixWidth;


}
void CPlotData::AppendData(const double fData)
{
	
}
//==================�����汾һ=====================

//void CPlotData::AppendData(const QVector<double> vctData,bool bChange)
//{
//	if (vctData.isEmpty())
//		return;
//	//��ȡ��ǰ����
//	int i_index = GetCurrentIndex();
//	//��ȡ����
//	double f_cur_ration = GetRatio().first;
//	double f_next_ration = GetRatio().second;
//
//	//�ж������Ƿ���һ�������Ҵﵽ��ǰ�����µ������������
//	//��һ������յ�ǰ���У�����һ��������ӵ���ǰ����
//	//////////////////////////////////////////////////////////////////////////////////////////////
//	/*�ŵ㣺�˷�����Ч�Ľ���ʾ���ݺ�ԭʼ���ݷ��뿪��������Ϊԭʼ����������ռ�ù����ڴ�			*/
//	/*ȱ�㣺��������һ��ʱ��ʱ������չΪ��һ����2�����ϣ�ͼ��Ż���ʾ�������Σ�����ͼ���ʧ��	*/
//	/*�����i�����ݶ�Ӧԭʼ���ݵ�a11��a1n֡,��i + 1�����ݶ�Ӧԭʼ���ݵ� a21��a2n֡������ǰʱ����*/
//	/*������һ����2������ʱ��a21 < a1n �����м����ݶ�ʧ�����ͼ��ʧ��							*/
//	//////////////////////////////////////////////////////////////////////////////////////////////
//	if (m_vctCurEvn.size() == m_iPixWidth && bChange)
//	{
//		m_vctCurEvn.clear();
//		m_vctCurEvn.append(m_vctNextEvn);
//		m_vctNextEvn.clear();
//		//��ʽ1������ǰ���ݿ������µ�
//		for(int i = 0; i< m_vctCurEvn.size(); ++i)
//		{
//			QPair<double,double> pair_data = m_vctCurEvn.at(i);
//			//ԭ�����е�i�ж�Ӧ��������к�
//			int i_max_index = qCeil(f_cur_ration * (i + 1)) - 1;
//			
//			int i_raw_index = i_max_index > i_index ? i_index : i_max_index ;
//			int i_cur_col = i_raw_index / f_next_ration;
//			if(i_cur_col == m_vctNextEvn.size())
//			{
//				m_vctNextEvn.append(pair_data);
//			}
//			//step1:ȡ��һ�е�ԭ����ֵ
//			double f_low = m_vctNextEvn.at(i_cur_col).first;
//			//step2:�ж�ԭʼֵ�Ƿ�Ϊ��Чֵ
//			if (C_VACANT_VALUE == f_low || C_INVALID_VALUE == f_low)
//			{
//				m_vctNextEvn[i_cur_col].first = pair_data.first;
//			}
//			//step3:�ж�f_low�Ƿ�Ϊ��Чֵ
//			else if (C_INVALID_VALUE == pair_data.first)
//			{
//				m_vctNextEvn[i_cur_col].first = f_low;
//			}
//			else
//			{
//				m_vctNextEvn[i_cur_col].first = pair_data.first < f_low ? pair_data.first : f_low;
//			}
//			double f_high = m_vctNextEvn.at(i_cur_col).second;
//			if (C_VACANT_VALUE == f_high || C_INVALID_VALUE == f_high)
//			{
//				m_vctNextEvn[i_cur_col].second = pair_data.second;
//			}
//			else if (C_INVALID_VALUE == pair_data.second)
//			{
//				m_vctNextEvn[i_cur_col].second = f_high;
//			}
//			else
//			{
//				m_vctNextEvn[i_cur_col].second = pair_data.second > f_high ? pair_data.second : f_high;
//			}
//		}
//
//	}
//
//	//������ʾ����
//	for (int i = 0; i < vctData.size(); ++i)
//	{
//		double f_data = vctData.at(i);
//		int i_cur_col = double(i + i_index) / f_cur_ration;
//		int i_next_col = double(i + i_index) / f_next_ration;
//
//		//�ж��Ƿ�������,������ֱ�����
//		if (i_cur_col == m_vctCurEvn.size())
//		{
//			m_vctCurEvn.append(QPair<double, double>(f_data, f_data));
//		}
//		//��������
//		else
//		{
//			//step1:ȡ��һ�е�ԭ����ֵ
//			double f_low = m_vctCurEvn.at(i_cur_col).first;
//			//step2:�ж�ԭʼֵ�Ƿ�Ϊ��Чֵ
//			if (C_VACANT_VALUE == f_low || C_INVALID_VALUE == f_low)
//			{
//				m_vctCurEvn[i_cur_col].first = f_data;
//			}
//			//step3:�ж�f_low�Ƿ�Ϊ��Чֵ
//			else if (C_INVALID_VALUE == f_data)
//			{
//				m_vctCurEvn[i_cur_col].first = f_low;
//			}
//			else
//			{
//				m_vctCurEvn[i_cur_col].first = f_data < f_low ? f_data : f_low;
//			}
//			double f_high = m_vctCurEvn.at(i_cur_col).second;
//			if (C_VACANT_VALUE == f_high || C_INVALID_VALUE == f_high)
//			{
//				m_vctCurEvn[i_cur_col].second = f_data;
//			}
//			else if (C_INVALID_VALUE == f_data)
//			{
//				m_vctCurEvn[i_cur_col].second = f_high;
//			}
//			else
//			{
//				m_vctCurEvn[i_cur_col].second = f_data > f_high ? f_data : f_high;
//			}
//		}
//		//������һ�����ʵĴ洢
//		//�ж��Ƿ�����
//		if (i_next_col == m_vctNextEvn.size())
//		{
//			m_vctNextEvn.append(QPair<double, double>(f_data, f_data));
//		}
//		else
//		{
//			//step1:ȡ��һ�е�ԭ����ֵ
//			double f_low = m_vctNextEvn.at(i_next_col).first;
//			//step2:�ж�ԭʼֵ�Ƿ�Ϊ��Чֵ
//			if (C_VACANT_VALUE == f_low || C_INVALID_VALUE == f_low)
//			{
//				m_vctNextEvn[i_next_col].first = f_data;
//			}
//			//step3:�ж�f_low�Ƿ�Ϊ��Чֵ
//			else if (C_INVALID_VALUE == f_data)
//			{
//				m_vctNextEvn[i_next_col].first = f_low;
//			}
//			else
//			{
//				m_vctNextEvn[i_next_col].first = f_data < f_low ? f_data : f_low;
//			}
//			double f_high = m_vctNextEvn.at(i_next_col).second;
//			if (C_VACANT_VALUE == f_high || C_INVALID_VALUE == f_high)
//			{
//				m_vctNextEvn[i_next_col].second = f_data;
//			}
//			else if (C_INVALID_VALUE == f_data)
//			{
//				m_vctNextEvn[i_next_col].second = f_high;
//			}
//			else
//			{
//				m_vctNextEvn[i_next_col].second = f_data > f_high ? f_data : f_high;
//			}
//		}
//		AddIndex(1);
//	}
//}

//==================�����汾��=====================
void CPlotData::AppendData(const QVector<double> vctData,const CPlotXMap &cAxisX,const bool &bChange)
{
	//�����ж�
	if (vctData.empty())
		return;
	//��ǰ���ݳ���
	int i_index = GetCurrentIndex();
	//���������֮ǰ��ʱ����ĳ����Ѿ�����չ
	double f_cur_ratio = m_fCurRatio;
	
	//�ж���ʾ�����Ƿ���һ�����Ƿ���Ҫ����
	if(bChange)
	{
		double f_pre_ratio = m_fCurRatio * m_fPreScale;
		//�޸���ʾֵ
		//ModifyShowData(QPair<double,double>(f_pre_ratio,f_cur_ratio));
		MoveCurve();
	}
	//�������
	for(int i = 0; i < vctData.size(); ++i)
	{
		int i_col = (i_index + i) / f_cur_ratio - cAxisX.GetBegCol(); 
		if(i_col == m_vctCurData.size())
		{
			m_vctCurData.append(QPair<double,double>(vctData.value(i),vctData.value(i)));
		}
		else
		{
			double f_low = m_vctCurData.at(i_col).first;
			m_vctCurData[i_col].first = vctData.value(i) < f_low ? vctData.value(i) : f_low;
			double f_high = m_vctCurData.at(i_col).second;
			m_vctCurData[i_col].second = vctData.value(i) >f_high ? vctData.value(i) : f_high;
		}
		AddIndex(1);
	}

	m_vctRawData.append(vctData);
}
//��ȡ���ݵ��ϰ���
QVector<double> CPlotData::GetDisplayUpEvn(const CPlotXMap &xMap) const 
{
	QVector <double> vct_env;
	for (auto it : m_vctCurData)
	{
		vct_env.append(it.first);
	}
	return vct_env;
}
//��ȡ���ݵ��°���
QVector<double> CPlotData::GetDisplayDowmEvn(const CPlotXMap &xMap) const
{
	QVector<double> vct_env;
	for (auto it : m_vctCurData)
	{
		vct_env.append(it.second);
	}
	return vct_env;
}

//�޸���ʾֵm_vctCurEvn=======================================
void CPlotData::ModifyShowData(const QPair<double,double> &pairRatio)
{
	//�����ж�
	Q_ASSERT(pairRatio.first > 0 && pairRatio.second > 0);
	if(m_vctCurData.empty())
	{
		return;
	}
	double i_pre_ratio = pairRatio.first;
	double i_cur_ratio = pairRatio.second;
	QVector<QPair<double, double>> vct_value;
	//ʹ����ʾ���ݻ�������
	for(int i = 0; i< m_vctCurData.size(); ++i)
	{
		int i_col = m_fPreScale * i;
		if( i_col >vct_value.size())
		{
			qDebug() << "error";
		}
		if( i_col == vct_value.size())
		{
			vct_value.append(m_vctCurData.value(i));
		}
		else
		{
			double f_low = m_vctCurData.at(i).first;
			vct_value[i_col].first = f_low < vct_value.at(i_col).first ? f_low : vct_value.at(i_col).first;
			double f_high = m_vctCurData.at(i).second;
			vct_value[i_col].second = f_high > vct_value.at(i_col).second ? f_high : vct_value.at(i_col).second;
		}
	}

	//����ԭʼ���ݻ�������
	/*for(int i = 0; i< m_vctRawData.size(); ++i)
	{
		int i_col = ceil(i / i_cur_ratio);
		if(i_col > vct_value.size())
		{
			qDebug() << "error";
		}
		if(i_col == vct_value.size())
		{
			vct_value.append(QPair<double,double>(m_vctRawData.value(i),m_vctRawData.value(i)));
		}
		else
		{
			double f_low = m_vctRawData.value(i);
			vct_value[i_col].first = f_low < vct_value.at(i_col).first ? f_low : vct_value.at(i_col).first;
			double f_high = m_vctRawData.value(i);
			vct_value[i_col].second = f_high > vct_value.at(i_col).second ? f_high : vct_value.at(i_col).second;
		}
	}*/
	m_vctCurData.clear();
	m_vctCurData.append(vct_value);
	
}

//==================ƽ������add <2020/10/9> xiongz=====================
void CPlotData::MoveCurve()
{
	if(m_vctCurData.empty())
	{
		return;
	}
	QVector<QPair<double, double>> vct_tmp(m_vctCurData);
	m_vctCurData.clear();
	//����ƽ��Ԥm_iStep��λ
	for(int i = m_iStep - 1; i < vct_tmp.size(); ++i)
	{
		m_vctCurData.append(vct_tmp.value(i));
	}
}