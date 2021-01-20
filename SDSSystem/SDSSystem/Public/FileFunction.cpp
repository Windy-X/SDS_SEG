/********************************************************************
* Copyright (C) 深圳市德力凯医疗设备股份有限公司
* All rights reserved.
*
* 文件名称：FileFunction.cpp
* 摘    要：
*		文件处理工具实现
* 历史记录：
*		<2020/04/21> dingmh, Created
*********************************************************************/
#include "FileFunction.h"
#include <QFile>
#include <QDir>
#include <QCoreApplication>
#include <QTextCodec>
#include "PublicDefs.h"

namespace file_func
{
bool IsFileExist(const std::string &strFileName)
{
	QTextCodec *codec = QTextCodec::codecForLocale();
	return QFile::exists(codec->toUnicode(strFileName.c_str()));
}

bool IsFileExist(const QString &strFileName)
{
	return QFile::exists(strFileName);
}

bool IsDirExist(const std::string &strDir)
{
	QTextCodec *codec = QTextCodec::codecForLocale();
	QDir dir(codec->toUnicode(strDir.c_str()));
	return dir.exists();
}

bool IsDirExist(const QString &strDir)
{
	QDir dir(strDir);
	return dir.exists();
}

bool MakeDir(const std::string &strDir)
{
	QTextCodec *codec = QTextCodec::codecForLocale();
	QDir dir;
	return dir.mkpath(codec->toUnicode(strDir.c_str()));
}

bool MakeDir(const QString &strDir)
{
	QDir dir;
	return dir.mkpath(strDir);
}

bool RenameDir(const std::string &strOldDir, const std::string &strNewDir)
{
	QTextCodec *codec = QTextCodec::codecForLocale();
	QDir dir;
	return dir.rename(codec->toUnicode(strOldDir.c_str()), codec->toUnicode(strNewDir.c_str()));
}

bool RenameDir(const QString &strOldDir, const QString &strNewDir)
{
	QDir dir;
	return dir.rename(strOldDir, strNewDir);
}

QString GetExePath()
{
	QString strpath = QCoreApplication::applicationDirPath();
	return strpath;
}

bool RemoveFile(const std::string &strFileName)
{
	if (!IsFileExist(strFileName))
	{
		return true;
	}
	QTextCodec *codec = QTextCodec::codecForLocale();
	QFile file(codec->toUnicode(strFileName.c_str()));
	if (file.isOpen())
	{
		file.close();
	}
	return file.remove();
}

bool RemoveFile(const QString &strFileName)
{
	if (!IsFileExist(strFileName))
	{
		return true;
	}
	QFile file(strFileName);
	if (file.isOpen())
	{
		file.close();
	}
	return file.remove();
}

bool RemoveDir(const std::string &strDir)
{
	QTextCodec *codec = QTextCodec::codecForLocale();
	QDir dir(codec->toUnicode(strDir.c_str()));
	return dir.removeRecursively();
}

bool RemoveDir(const QString &strDir)
{
	QDir dir(strDir);
	return dir.removeRecursively();
}

int CopyFileToFile(const std::string& strSrc, const std::string& strDest, bool bOverwriteExist)
{
	if (IsFileExist(strDest) && !bOverwriteExist)
	{
		return ERR_FILE_ALREADY_EXISTS;
	}
	if (!IsFileExist(strSrc))
	{
		return ERR_FILE_NOT_EXISTS;
	}

	RemoveFile(strDest);
	QTextCodec *codec = QTextCodec::codecForLocale();
	if (!QFile::copy(codec->toUnicode(strSrc.c_str()), codec->toUnicode(strDest.c_str())))
	{
		return ERR_FILE_PROCESSING_FAILED;
	}
	return ERR_OK;
}

int CopyFileToFile(const QString &strSrc, const QString &strDest, bool bOverwriteExist)
{
	if (IsFileExist(strDest) && !bOverwriteExist)
	{
		return ERR_FILE_ALREADY_EXISTS;
	}
	if (!IsFileExist(strSrc))
	{
		return ERR_FILE_NOT_EXISTS;
	}

	RemoveFile(strDest);
	if (!QFile::copy(strSrc, strDest))
	{
		return ERR_FILE_PROCESSING_FAILED;
	}
	return ERR_OK;
}
}