#include "PlotSegmentData.h"
#include <QDebug>
CPlotSegmentData* CPlotSegmentData::m_pInstance = NULL;
QMutex CPlotSegmentData::m_Mutex;
QMutex CPlotSegmentData::m_MutexSeg;
QMutex CPlotSegmentData::m_MutexEvent;
CPlotSegmentData::CPlotSegmentData()
	 {
		m_vctSeg.clear();
	 }
CPlotSegmentData* CPlotSegmentData::GetInstance()
{
	if(NULL == m_pInstance)
	{
		QMutexLocker locker(&m_Mutex);
		if(NULL == m_pInstance)
		{
			m_pInstance = new CPlotSegmentData;
		}
	}
	return m_pInstance;
}
void CPlotSegmentData::ReleaseInstance()
{
	if(NULL != m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}
 CPlotSegmentData::~CPlotSegmentData()
 {
	 Clear();
 }
 //清除资源
 void CPlotSegmentData::Clear()
 {
	 m_vctSeg.clear();
	 m_vctEvent.clear();
 }
 //初始化
 void CPlotSegmentData::Init(const qint64& iWidth)
 {
	 //m_iPixWidth = iWidth;
 }
 void CPlotSegmentData::Init()
 {
	 CSegmentData::Instance()->SetDir(file_func::GetExePath());
	 CSegmentData::Instance()->SetFileName( C_SEGMENT_XML_FILE_NAME);
 }
 void CPlotSegmentData::AppendSeg(const SegmentMarks& stSeg)
 {
	 QMutexLocker locker(&m_MutexSeg);
	 UpdateSegemntMarkList();
	 for (int i = 0; i < m_vctSeg.size(); ++i)
	 {
		 if (stSeg.eFlag == m_vctSeg.value(i).eFlag)
		 {
			 m_vctSeg.replace(i, stSeg);
			 return;
		 }
	}
	 m_vctSeg.append(stSeg);
 }
 void CPlotSegmentData::AppendSeg(const QVector<SegmentMarks>& vctSeg)
 {
	 Q_ASSERT(!vctSeg.empty());
	 QMutexLocker locker(&m_MutexSeg);
	 m_vctSeg.append(vctSeg);
 }
 //数据帧号转换为当前列号
 qint64 CPlotSegmentData::TransFrame2Col(const qint64& iFrame)
 {
	 int i_col = (iFrame < 0) ? 0 : iFrame;
	 i_col =  i_col / GetPageRatio();
	 return i_col;

 }
 //选段框列号转换为当前数据帧号
 qint64 CPlotSegmentData::TransCol2Frame(const qint64 &iCol)
 {
	 int i_frm = (iCol < 0) ? 0 : iCol;
	 i_frm = ceil(i_frm * GetPageRatio());
	 return i_frm;
 }
 //删除选段记录
 void CPlotSegmentData::DeleteSeg(const qint64 &iIndex)
 {
	 QMutexLocker locker(&m_MutexSeg);
	 //参数判断
	 if (iIndex >= m_vctSeg.size())
	 {
		 qDebug() << "can't find recorder in vector";
		 return;
	 }
	 m_vctSeg.removeAt(iIndex);

 }
 //控件选段，转换选定位置到数据帧号,
 void CPlotSegmentData::AddSegment(const int &iBegCol,const int &iEndCol, const QColor &color)
 {
	 //参数判断
	 int i_beg_col = iBegCol < 0 ? 0 : iBegCol;
	 int i_end_col = iEndCol > m_cAxisX.GetEndCol() ? m_cAxisX.GetEndCol() : iEndCol;
	 //计算
	 QMutexLocker locker(&m_MutexSeg);
	 SegmentMarks st_seg;
	 st_seg.iBegIndex = m_cAxisX.InvTrans(i_beg_col);
	 st_seg.iEndIndex = m_cAxisX.InvTrans(i_end_col);
	 st_seg.stView.colorBackground = color;
	 m_vctSeg.append(st_seg);
	 //在排序前将最后一个元素保存下来，用于修改背景色	 @xiongz [2020/11/5]
	 m_stCurSeg = m_vctSeg.last();
	 qSort(m_vctSeg.begin(),m_vctSeg.end(),SegmentMarks::CompareSeg);

 }
//设置时间轴
 void CPlotSegmentData::SetTimeMap(const CPlotXMap &cAxisX)
 {
	 m_cAxisX = cAxisX;
 }

 //====================添加事件标记
 void CPlotSegmentData::AppendEventMark(const StampedEvent &stEvent)
 {
	 //TODO:查重
	 QMutexLocker locker(&m_MutexEvent);
	 for(QVector<StampedEvent>::iterator it = m_vctEvent.begin();
		 it != m_vctEvent.end();
		 ++ it)
	 {
		 if (stEvent.iIndex == it->iIndex)
		 {
			 *it = stEvent;
			 return;
		 }
	 }
	 m_vctEvent.append(stEvent);
	 qSort(m_vctEvent.begin(),m_vctEvent.end(),StampedEvent::CompareEvent);
 }
 void CPlotSegmentData::AppendEventMark(const QVector<StampedEvent> &vctEvent)
 {
	 for (auto it : vctEvent)
	 {
		// AppendEventMark(it);
	 }
 }
 void CPlotSegmentData::DeleteEvent(const qint64 &iIndex,const E_EVENT_TYPE &eType)
 {
	 for (int i = 0; i < m_vctEvent.size(); ++i)
	 {
		 if (iIndex == m_vctEvent[i].iIndex && eType == m_vctEvent[i].eType)
		 {
			 m_vctEvent.remove(i);
			 break;
		 }
	 }
 }
 void CPlotSegmentData::SetCurSegBackground(const QColor &color)
 {
	 m_stCurSeg.stView.colorBackground = color;
	 for (QVector<SegmentMarks>::iterator it = m_vctSeg.begin();
		 it != m_vctSeg.end();
		 ++ it)
	 {
		 if (m_stCurSeg.iBegIndex == it->iBegIndex &&
			 m_stCurSeg.iEndIndex == it->iEndIndex)
		 {
			 it->stView.colorBackground = m_stCurSeg.stView.colorBackground;
			 break;
		 }
	 }
 }
 //===================选段保存==================
 void CPlotSegmentData::SaveSegmentMarks()
 {
	 UpdateSegemntMarkList();
	 CSegmentData::Instance()->SaveSegmentMarks(m_vctSeg);
 }
 void CPlotSegmentData::UpdateSegemntMarkList()
 {

	 for (int i = 0; i < m_vctSeg.size(); ++i)
	 {
		 if (E_FLAG_NULL == m_vctSeg.value(i).eFlag)
		 {
			 m_vctSeg.remove(i);
		 }
	 }
 }