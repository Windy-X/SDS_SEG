#pragma once
#include <QMutex>
#include "PublicDefs.h"
#include "FileFunction.h"
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"
class CEventData
{
public:
	static CEventData *Instance();
	static void ReleaseInstance();
	//设置/获取文件路径
	void SetDir(const QString &strDir);
	QString GetDir() const;
	//设置/获取文件名
	void SetFileName(const QString &strFileName);
	QString GetFileName() const;
	//设置/获取事件列表
	void SetEventList(const QVector<StampedEvent> &);
	QVector<StampedEvent> GetEventList() const;
	//存储/读取时间列表

private:
	static QMutex m_Mutex;
	static QMutex m_MutexEventList;
	static CEventData *m_pInstance;
	QString m_strDir;		//文件路径
	QString m_strFileName;	//文件名
	QVector<StampedEvent> m_vctEventList;
};

