#include "diffPluginWrapper.h"

#include <QtWidgets/QTabWidget>

#include <plugins/tools/visualDiff/model/diffModel.h>
#include <plugins/tools/visualDiff/view/diffWindow.h>


using namespace versioning;
using namespace qReal;

DiffPluginWrapper::DiffPluginWrapper()
{
}

Customizer *DiffPluginWrapper::customizationInterface()
{
	return nullptr;
}

void DiffPluginWrapper::updateSettings()
{
}

QList<ActionInfo> DiffPluginWrapper::actions()
{
	return QList<qReal::ActionInfo>();
}

void DiffPluginWrapper::init(const PluginConfigurator &configurator)
{
	Q_UNUSED(configurator)
}

QPair<QString, gui::PreferencesPage *> DiffPluginWrapper::preferencesPage()
{
	///to do: make color options
	return QPair<QString, gui::PreferencesPage *>();
}

void DiffPluginWrapper::configure(
	ProjectManagementInterface *projectManager
	, ErrorReporterInterface *errorReporter
	, qrRepo::WorkingCopyManagementInterface *workingCopyManager
	, BriefVersioningInterface *vcs
	, QWidget *parent
	, EditorManagerInterface *manager
)
{
	mWorkingCopyManager = workingCopyManager;
	mVcs = vcs;
	mMainWindow = parent;
	mEditorManager = manager;
	mErrorReporter = errorReporter;
	mProjectManager = projectManager;
	mLoader = new details::ModelLoader(mVcs, mErrorReporter, mEditorManager, mWorkingCopyManager);
	connect(mLoader, SIGNAL(modelLoaded(DiffModel*)), this, SLOT(onModelLoaded(DiffModel *)));
}

void DiffPluginWrapper::showDiff(const QString &targetProject, QWidget *parentWidget, bool compactMode)
{
	mCompactMode = compactMode;
	mParentWidget = parentWidget;
	solveConflicts = false;
	mLoader->startModelLoading(targetProject);
}

void DiffPluginWrapper::showDiff(
	const QString &repoRevision
	, const QString &targetProject
	, QWidget *parentWidget
	, bool compactMode
)
{
	mCompactMode = compactMode;
	mParentWidget = parentWidget;
	solveConflicts = false;
	mLoader->startModelLoading(repoRevision, targetProject);
}

void DiffPluginWrapper::showDiff(
	const QString &oldRepoRevision
	, const QString &newRepoRevision
	, const QString &targetProject
	, QWidget *parentWidget
	, bool compactMode
)
{
	mCompactMode = compactMode;
	mParentWidget = parentWidget;
	solveConflicts = false;
	mLoader->startModelLoading(oldRepoRevision, newRepoRevision, targetProject);
}

void DiffPluginWrapper::solvePull(
	const QString &repoUrl
	, const QString &branch
	, const QString &targetProject
	, QWidget *parentWidget
)
{
	mIsPull = true;
	mParentWidget = parentWidget;
	mTargetProject = targetProject;
	solveConflicts = true;
	mLoader->startModelLoading2(repoUrl, branch, targetProject);
}

void DiffPluginWrapper::solveMerge(const QString &targetBranch, const QString &targetProject, QWidget *parentWidget)
{
	mIsPull = false;
	mParentWidget = parentWidget;
	mTargetProject = targetProject;
	solveConflicts = true;
	mLoader->startModelLoading2(targetBranch, targetProject);
}

void DiffPluginWrapper::onModelLoaded(DiffModel *model)
{
	if (!model) {
		return;
	}

	mDiffModel = model;

	if (!solveConflicts) {
		int newDiagrams = model->newModel()->graphicalModelAssistApi().childrenOfRootDiagram();
		int oldDiagrams = model->oldModel()->graphicalModelAssistApi().childrenOfRootDiagram();
		int diagrams = std::min(newDiagrams, oldDiagrams);
		QTabWidget *diffWindowSet = new QTabWidget;
		for (int i = 0; i < diagrams; i++){
			DiffWindow *tmp = new DiffWindow(model, mCompactMode, i, mMainWindow);
			QString text = QString(tr("diagram ")) + QString::number(i+1);
			if (tmp->diagramChanged()) {
				text += QString("_CHANGED*");
			}
			diffWindowSet->addTab(tmp, text);
		}

		mParentWidget->layout()->addWidget(diffWindowSet);
	} else {
		mConflictWidget = new ConflictsWindow(model, mMainWindow);
		connect(mConflictWidget, SIGNAL(processWorkingCopy()), SLOT(processProjectAndReopen()));
		mParentWidget->layout()->addWidget(mConflictWidget);
	}
}

void DiffPluginWrapper::processProjectAndReopen()
{
	mDiffModel->oldActiveModel()->repoControlApi().saveTo(mTargetProject);
	if (mIsPull) {
		emit pullOurs();
	} else {
		emit mergeOurs();
	}
}

