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
	//设置绘图区像素宽度
	m_iPixWidth = iPixWidth;
	//获取预留窗口宽度
	m_iReseverdWidth = CConfig::Instance()->GetReservedWidth();
	m_fPreScale = double(m_iPixWidth - m_iReseverdWidth) / m_iPixWidth;
	m_fNextScale = double(m_iPixWidth) / (m_iPixWidth - m_iReseverdWidth);
	//设置前进步进 add <2020/10/09> xiongz
	m_iStep = m_iReseverdWidth;

	//debug，自动给定配置文件中的值，待删除修改 debug <2020/9/29>xiongz
	m_iTimeWidth = CConfig::Instance()->GetPageDataLen(0);
	m_fCurRatio = double(m_iTimeWidth) / m_iPixWidth;


}
void CPlotData::AppendData(const double fData)
{
	
}
//==================函数版本一=====================

//void CPlotData::AppendData(const QVector<double> vctData,bool bChange)
//{
//	if (vctData.isEmpty())
//		return;
//	//获取当前索引
//	int i_index = GetCurrentIndex();
//	//获取比率
//	double f_cur_ration = GetRatio().first;
//	double f_next_ration = GetRatio().second;
//
//	//判断数据是否满一屏，并且达到当前比率下的最大数据索引
//	//满一屏则清空当前序列，将下一档数据添加到当前序列
//	//////////////////////////////////////////////////////////////////////////////////////////////
//	/*优点：此方法有效的将显示数据和原始数据分离开，不会因为原始数据量过大占用过多内存			*/
//	/*缺点：当数据满一屏时，时间轴拓展为上一档的2倍以上，图像才会显示正常波形，否则图像会失真	*/
//	/*比如第i列数据对应原始数据的a11到a1n帧,第i + 1列数据对应原始数据的 a21到a2n帧，当当前时间轴*/
//	/*不是上一档的2倍以上时，a21 < a1n 导致中间数据丢失，造成图像失真							*/
//	//////////////////////////////////////////////////////////////////////////////////////////////
//	if (m_vctCurEvn.size() == m_iPixWidth && bChange)
//	{
//		m_vctCurEvn.clear();
//		m_vctCurEvn.append(m_vctNextEvn);
//		m_vctNextEvn.clear();
//		//方式1：将当前数据拷贝到下档
//		for(int i = 0; i< m_vctCurEvn.size(); ++i)
//		{
//			QPair<double,double> pair_data = m_vctCurEvn.at(i);
//			//原序列中第i列对应的最大序列号
//			int i_max_index = qCeil(f_cur_ration * (i + 1)) - 1;
//			
//			int i_raw_index = i_max_index > i_index ? i_index : i_max_index ;
//			int i_cur_col = i_raw_index / f_next_ration;
//			if(i_cur_col == m_vctNextEvn.size())
//			{
//				m_vctNextEvn.append(pair_data);
//			}
//			//step1:取这一列的原来的值
//			double f_low = m_vctNextEvn.at(i_cur_col).first;
//			//step2:判断原始值是否为无效值
//			if (C_VACANT_VALUE == f_low || C_INVALID_VALUE == f_low)
//			{
//				m_vctNextEvn[i_cur_col].first = pair_data.first;
//			}
//			//step3:判断f_low是否为无效值
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
//	//增加显示数据
//	for (int i = 0; i < vctData.size(); ++i)
//	{
//		double f_data = vctData.at(i);
//		int i_cur_col = double(i + i_index) / f_cur_ration;
//		int i_next_col = double(i + i_index) / f_next_ration;
//
//		//判断是否是新列,是新列直接添加
//		if (i_cur_col == m_vctCurEvn.size())
//		{
//			m_vctCurEvn.append(QPair<double, double>(f_data, f_data));
//		}
//		//不是新列
//		else
//		{
//			//step1:取这一列的原来的值
//			double f_low = m_vctCurEvn.at(i_cur_col).first;
//			//step2:判断原始值是否为无效值
//			if (C_VACANT_VALUE == f_low || C_INVALID_VALUE == f_low)
//			{
//				m_vctCurEvn[i_cur_col].first = f_data;
//			}
//			//step3:判断f_low是否为无效值
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
//		//计算下一档比率的存储
//		//判断是否新列
//		if (i_next_col == m_vctNextEvn.size())
//		{
//			m_vctNextEvn.append(QPair<double, double>(f_data, f_data));
//		}
//		else
//		{
//			//step1:取这一列的原来的值
//			double f_low = m_vctNextEvn.at(i_next_col).first;
//			//step2:判断原始值是否为无效值
//			if (C_VACANT_VALUE == f_low || C_INVALID_VALUE == f_low)
//			{
//				m_vctNextEvn[i_next_col].first = f_data;
//			}
//			//step3:判断f_low是否为无效值
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

//==================函数版本二=====================
void CPlotData::AppendData(const QVector<double> vctData,const CPlotXMap &cAxisX,const bool &bChange)
{
	//参数判断
	if (vctData.empty())
		return;
	//当前数据长度
	int i_index = GetCurrentIndex();
	//在添加数据之前，时间轴的长度已经被拓展
	double f_cur_ratio = m_fCurRatio;
	
	//判断显示数据是否满一屏，是否需要换挡
	if(bChange)
	{
		double f_pre_ratio = m_fCurRatio * m_fPreScale;
		//修改显示值
		//ModifyShowData(QPair<double,double>(f_pre_ratio,f_cur_ratio));
		MoveCurve();
	}
	//添加数据
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
//获取数据的上包络
QVector<double> CPlotData::GetDisplayUpEvn(const CPlotXMap &xMap) const 
{
	QVector <double> vct_env;
	for (auto it : m_vctCurData)
	{
		vct_env.append(it.first);
	}
	return vct_env;
}
//获取数据的下包络
QVector<double> CPlotData::GetDisplayDowmEvn(const CPlotXMap &xMap) const
{
	QVector<double> vct_env;
	for (auto it : m_vctCurData)
	{
		vct_env.append(it.second);
	}
	return vct_env;
}

//修改显示值m_vctCurEvn=======================================
void CPlotData::ModifyShowData(const QPair<double,double> &pairRatio)
{
	//参数判断
	Q_ASSERT(pairRatio.first > 0 && pairRatio.second > 0);
	if(m_vctCurData.empty())
	{
		return;
	}
	double i_pre_ratio = pairRatio.first;
	double i_cur_ratio = pairRatio.second;
	QVector<QPair<double, double>> vct_value;
	//使用显示数据绘制曲线
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

	//利用原始数据绘制曲线
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

//==================平移曲线add <2020/10/9> xiongz=====================
void CPlotData::MoveCurve()
{
	if(m_vctCurData.empty())
	{
		return;
	}
	QVector<QPair<double, double>> vct_tmp(m_vctCurData);
	m_vctCurData.clear();
	//向左平移预m_iStep单位
	for(int i = m_iStep - 1; i < vct_tmp.size(); ++i)
	{
		m_vctCurData.append(vct_tmp.value(i));
	}
}