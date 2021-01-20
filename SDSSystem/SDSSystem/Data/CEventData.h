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
	//����/��ȡ�ļ�·��
	void SetDir(const QString &strDir);
	QString GetDir() const;
	//����/��ȡ�ļ���
	void SetFileName(const QString &strFileName);
	QString GetFileName() const;
	//����/��ȡ�¼��б�
	void SetEventList(const QVector<StampedEvent> &);
	QVector<StampedEvent> GetEventList() const;
	//�洢/��ȡʱ���б�

private:
	static QMutex m_Mutex;
	static QMutex m_MutexEventList;
	static CEventData *m_pInstance;
	QString m_strDir;		//�ļ�·��
	QString m_strFileName;	//�ļ���
	QVector<StampedEvent> m_vctEventList;
};

