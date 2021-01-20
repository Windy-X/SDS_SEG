/********************************************************************
* Copyright (C) 深圳市德力凯医疗设备股份有限公司
* All rights reserved.
*
* 文件名称：FileFunction.h
* 摘    要：
*		文件处理工具
* 历史记录：
*		<2020/04/21> dingmh, Created
*********************************************************************/
#pragma once
#include <string>
#include <QString>

namespace file_func
{
// 判断文件是否存在
bool IsFileExist(const std::string &strFileName);
bool IsFileExist(const QString &strFileName);
// 判断文件夹是否存在
bool IsDirExist(const std::string &strDir);
bool IsDirExist(const QString &strDir);
// 创建文件夹
bool MakeDir(const std::string &strDir);
bool MakeDir(const QString &strDir);
// 重命名文件夹
bool RenameDir(const std::string &strOldDir, const std::string &strNewDir);
bool RenameDir(const QString &strOldDir, const QString &strNewDir);
// 获取运行路径
QString GetExePath();
// 删除文件
bool RemoveFile(const std::string &strFileName);
bool RemoveFile(const QString &strFileName);
// 删除文件夹
bool RemoveDir(const std::string &strDir);
bool RemoveDir(const QString &strDir);
// 拷贝文件
int CopyFileToFile(const std::string &strSrc, const std::string &strDest, bool bOverwriteExist);
int CopyFileToFile(const QString &strSrc, const QString &strDest, bool bOverwriteExist);
}

