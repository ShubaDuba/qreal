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

#include "createGroupCommand.h"

#include "models/logicalModelAssistApi.h"
#include "models/graphicalModelAssistApi.h"

using namespace qReal::commands;

CreateGroupCommand::CreateGroupCommand(models::LogicalModelAssistApi &logicalApi
		, models::GraphicalModelAssistApi &graphicalApi
		, models::Exploser &exploser
		, const Id &logicalParent
		, const Id &graphicalParent
		, const Id &id
		, bool isFromLogicalModel
		, const QPointF &position)
	: mLogicalApi(logicalApi)
	, mGraphicalApi(graphicalApi)
	, mExploser(exploser)
	, mLogicalParent(logicalParent)
	, mGraphicalParent(graphicalParent)
	, mId(id)
	, mIsFromLogicalModel(isFromLogicalModel)
	, mPosition(position)
	, mPattern(graphicalApi.editorManagerInterface().getPatternByName(id.element()))
{
	const QPointF size = mPattern.size();

	// Pattern nodes create may have hierarchic structure. So we must create them in correct order
	// (parent first, child after). Cycles in hierarchy and nodes with incorrect parent id are fully ignored
	QList<GroupNode> toCreate = mPattern.nodes();
	QSet<QString> consideredNodes;
	QMap<QString, Id> createdNodesIds;
	// If group node has no parent then it has 'global' one
	createdNodesIds[QString()] = graphicalParent;
	bool somethingChangedThisIteration = true;
	while (!toCreate.isEmpty() && somethingChangedThisIteration) {
		somethingChangedThisIteration = false;
		for (const GroupNode &node : toCreate) {
			if (!node.parent.isEmpty() && !consideredNodes.contains(node.parent)) {
				continue;
			}

			const Id element(id.editor(), id.diagram(), node.type, QUuid::createUuid().toString());
			createdNodesIds[node.id] = element;
			if (node.id == mPattern.rootNode()) {
				mRootId = element;
			}

			const QPointF nodePos(position.x() - size.x() / 2 + node.position.x()
					, position.y() + node.position.y());
			CreateElementCommand *createNodeCommand = new CreateElementCommand(
					logicalApi, graphicalApi, exploser, logicalParent
					, createdNodesIds[node.parent], element, isFromLogicalModel
					, mLogicalApi.editorManagerInterface().friendlyName(element.type()), nodePos);
			mNodeCommands[node.id] = createNodeCommand;
			addPreAction(createNodeCommand);
			consideredNodes << node.id;
			toCreate.removeAll(node);
			somethingChangedThisIteration = true;
		}
	}
	// TODO: display here error if toCreate still non-empty

	for (const GroupEdge &edge : mPattern.edges()) {
		const Id element(id.editor(), id.diagram(), edge.type, QUuid::createUuid().toString());
		CreateElementCommand *createEdgeCommand = new CreateElementCommand(
					logicalApi, graphicalApi, exploser, logicalParent, graphicalParent, element, isFromLogicalModel
					, mLogicalApi.editorManagerInterface().friendlyName(element.type()), QPointF());
		mEdgeCommands.append(createEdgeCommand);
		addPreAction(createEdgeCommand);
	}
}

bool CreateGroupCommand::execute()
{
	// Elements themselves were already created in pre-actions
	QMap<QString, Id> nodes;
	for (const QString &idInGroup : mNodeCommands.keys()) {
		const CreateElementCommand *createNodeCommand = mNodeCommands[idInGroup];
		const Id newElemId = createNodeCommand->result();
		nodes.insert(idInGroup, newElemId);
	}

	for (int i = 0; i < mEdgeCommands.count(); ++i) {
		const CreateElementCommand *createEdgeCommand = mEdgeCommands[i];
		const GroupEdge groupEdge = mPattern.edges()[i];
		const Id newEdgeId = createEdgeCommand->result();
		mGraphicalApi.setFrom(newEdgeId, nodes.value(groupEdge.from));
		mGraphicalApi.setTo(newEdgeId, nodes.value(groupEdge.to));
	}

	return true;
}

bool CreateGroupCommand::restoreState()
{
	return true;
}

qReal::Id CreateGroupCommand::rootId() const
{
	return mRootId;
}
