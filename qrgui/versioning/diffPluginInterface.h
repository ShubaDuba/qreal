/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include <QtCore/QObject>

#include <qrutils/versioningUtils/briefVersioningInterface.h>
#include <qrgui/plugins/toolPluginInterface/toolPluginInterface.h>
#include <qrgui/plugins/pluginManager/editorManagerInterface.h>

#include "qrgui/versioning/versioningPluginsManagerDeclSpec.h"

namespace qReal
{

class QRGUI_VERSIONING_PLUGINS_MANAGER_EXPORT DiffPluginInterface : public QObject, public ToolPluginInterface
{
	Q_OBJECT

public:
	virtual ~DiffPluginInterface() {}

	/// Must be implemented to show diff between working copy and HEAD revision
	/// in remote repository.
	virtual void showDiff(
		const QString &targetProject
		, QWidget *parentWidget
		, bool compactMode = false
	) = 0;

	/// Must be implemented to show diff between working copy and specified revision
	/// in remote repository.
	virtual void showDiff(
		const QString &repoRevision
		, const QString &targetProject
		, QWidget *parentWidget
		, bool compactMode = false
	) = 0;

	/// Must be implemented to show diff between two specified revisions
	/// in remote repository.
	virtual void showDiff(
		const QString &oldRepoRevision
		, const QString &newRepoRevision
		, const QString &targetProject
		, QWidget *parentWidget
		, bool compactMode = false
	) = 0;

	/// Must be implemented to solve pull conflicts
	virtual void solvePull(
		const QString &repoUrl
		, const QString &branch
		, const QString &targetProject
		, QWidget *parentWidget
	) = 0;

	/// Must be implemented to solve merge conflicts
	virtual void solveMerge(const QString &targetBranch, const QString &targetProject, QWidget *parentWidget) = 0;

	/// For plugin, because plugin's configurator cannot give necessary objects
	virtual void configure(
		ProjectManagementInterface *projectManager
		, ErrorReporterInterface *errorReporter
		, qrRepo::WorkingCopyManagementInterface *workingCopyManager
		, BriefVersioningInterface *vcs
		, QWidget *parent, EditorManagerInterface *manager
	) = 0;

signals:
	void pullOurs();
	void mergeOurs();

};

}
