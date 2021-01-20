/********************************************************************
* Copyright (C) �����е�����ҽ���豸�ɷ����޹�˾
* All rights reserved.
*
* �ļ����ƣ�FileFunction.h
* ժ    Ҫ��
*		�ļ�������
* ��ʷ��¼��
*		<2020/04/21> dingmh, Created
*********************************************************************/
#pragma once
#include <string>
#include <QString>

namespace file_func
{
// �ж��ļ��Ƿ����
bool IsFileExist(const std::string &strFileName);
bool IsFileExist(const QString &strFileName);
// �ж��ļ����Ƿ����
bool IsDirExist(const std::string &strDir);
bool IsDirExist(const QString &strDir);
// �����ļ���
bool MakeDir(const std::string &strDir);
bool MakeDir(const QString &strDir);
// �������ļ���
bool RenameDir(const std::string &strOldDir, const std::string &strNewDir);
bool RenameDir(const QString &strOldDir, const QString &strNewDir);
// ��ȡ����·��
QString GetExePath();
// ɾ���ļ�
bool RemoveFile(const std::string &strFileName);
bool RemoveFile(const QString &strFileName);
// ɾ���ļ���
bool RemoveDir(const std::string &strDir);
bool RemoveDir(const QString &strDir);
// �����ļ�
int CopyFileToFile(const std::string &strSrc, const std::string &strDest, bool bOverwriteExist);
int CopyFileToFile(const QString &strSrc, const QString &strDest, bool bOverwriteExist);
}

