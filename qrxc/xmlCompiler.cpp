/* Copyright 2007-2016 QReal Research Group
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

#include "xmlCompiler.h"

#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QDebug>

#include <qrutils/outFile.h>
#include <qrutils/xmlUtils.h>
#include <qrutils/stringUtils.h>

#include "editor.h"
#include "nameNormalizer.h"
#include "diagram.h"
#include "type.h"
#include "edgeType.h"
#include "nodeType.h"
#include "portType.h"
#include "enumType.h"

using namespace utils;

XmlCompiler::XmlCompiler()
{
	mResources = "<!DOCTYPE RCC><RCC version=\"1.0\">\n<qresource>\n";

	QDir dir;
	if (!dir.exists("generated")) {
		dir.mkdir("generated");
	}
	dir.cd("generated");
	if (!dir.exists("shapes")) {
		dir.mkdir("shapes");
	}
	dir.cd("..");
}

XmlCompiler::~XmlCompiler()
{
	qDeleteAll(mEditors);
}

bool XmlCompiler::compile(const QString &inputXmlFileName, const QString &sourcesRootFolder)
{
	const QFileInfo inputXmlFileInfo(inputXmlFileName);
	mPluginName = NameNormalizer::normalize(inputXmlFileInfo.completeBaseName());
	mCurrentEditor = inputXmlFileInfo.absoluteFilePath();
	mSourcesRootFolder = sourcesRootFolder;
	const QDir startingDir = inputXmlFileInfo.dir();
	if (!loadXmlFile(startingDir, inputXmlFileInfo.fileName())) {
		return false;
	}

	mPluginVersion = mEditors[mCurrentEditor]->version();

	generateCode();
	return true;
}

Editor* XmlCompiler::loadXmlFile(const QDir &currentDir, const QString &inputXmlFileName)
{
	QFileInfo fileInfo(inputXmlFileName);
	Q_ASSERT(fileInfo.fileName() == inputXmlFileName);

	QString fullFileName = currentDir.absolutePath() + "/" + inputXmlFileName;
	qDebug() << "Loading file started: " << fullFileName;

	if (mEditors.contains(fullFileName)) {
		Editor *editor = mEditors[fullFileName];
		if (editor->isLoaded()) {
			qDebug() << "File already loaded";
			return editor;
		} else {
			qDebug() << "ERROR: cycle detected";
			return nullptr;
		}
	} else {
		QString errorMessage;
		int errorLine = 0;
		int errorColumn = 0;
		QDomDocument inputXmlDomDocument = xmlUtils::loadDocument(fullFileName
			, &errorMessage, &errorLine, &errorColumn);
		if (!errorMessage.isEmpty()) {
			qCritical() << QString("(%1, %2):").arg(errorLine).arg(errorColumn)
					<< "Could not parse XML. Error:" << errorMessage;
		}

		Editor *editor = new Editor(inputXmlDomDocument, this);
		if (!editor->load(currentDir)) {
			qDebug() << "ERROR: Failed to load file";
			delete editor;
			return nullptr;
		}
		mEditors[fullFileName] = editor;
		return editor;
	}
}

Diagram * XmlCompiler::getDiagram(const QString &diagramName)
{
	foreach (Editor *editor, mEditors) {
		Diagram *diagram = editor->findDiagram(diagramName);
		if (diagram) {
			return diagram;
		}
	}
	return nullptr;
}

void XmlCompiler::generateCode()
{
	if (!mEditors.contains(mCurrentEditor)) {
		qDebug() << "ERROR: Main editor xml was not loaded, generation aborted";
		return;
	}

	generateElementClasses();
	generatePluginHeader();
	generatePluginSource();
	generateResourceFile();
}

void XmlCompiler::addResource(const QString &resourceName)
{
	if (!mResources.contains(resourceName))
		mResources += resourceName;
}

void XmlCompiler::generateElementClasses()
{
	OutFile outElements("generated/elements.h");

	generateAutogeneratedDisclaimer(outElements);

	outElements() << "#pragma once\n\n"
		<< "#include <QBrush>\n"
		<< "#include <QPainter>\n\n"
		<< "#include <qrgraph/queries.h>\n"
		<< "#include <qrutils/xmlUtils.h>\n"
		<< "#include <metaMetaModel/nodeElementType.h>\n"
		<< "#include <metaMetaModel/edgeElementType.h>\n"
		<< "#include <metaMetaModel/patternType.h>\n"
		<< "#include <metaMetaModel/labelProperties.h>\n\n"
		;

	for (const Diagram *diagram : mEditors[mCurrentEditor]->diagrams().values()) {
		for (Type * const type : diagram->types().values()) {
			type->generateCode(outElements);
		}
	}
}

void XmlCompiler::generatePluginHeader()
{
	QString fileName = "generated/pluginInterface.h";// mPluginName

	OutFile out(fileName);

	generateAutogeneratedDisclaimer(out);

	out() << "#pragma once\n"
		<< "\n"
		<< "#include <metaMetaModel/metamodel.h>\n"
		<< "\n"
		<< "class " << mPluginName << "Plugin : public QObject, public qReal::Metamodel\n"
		<< "{\n\tQ_OBJECT\n\tQ_INTERFACES(qReal::Metamodel)\n"
		<< "\tQ_PLUGIN_METADATA(IID \"" << mPluginName << "\")\n"
		<< "\n"
		<< "public:\n"
		<< "\n"
		<< "\t" << mPluginName << "Plugin();\n"
		<< "\n"
		<< "private:\n"
		<< "\tvoid initPlugin();\n"
		<< "\tvoid initMultigraph();\n"
		<< "\tvoid initNameMap();\n"
		<< "\tvoid initEnums();\n"
		<< "\tvoid initPaletteGroupsMap();\n"
		<< "\tvoid initPaletteGroupsDescriptionMap();\n"
		<< "\tvoid initShallPaletteBeSortedMap();\n"
		<< "};\n"
		<< "\n";
}

void XmlCompiler::generatePluginSource()
{
	QString fileName = "generated/pluginInterface.cpp"; //mPluginName

	OutFile out(fileName);

	generateAutogeneratedDisclaimer(out);
	generateIncludes(out);
	generateInitPlugin(out);
	generateEnumValues(out);
}

void XmlCompiler::generateAutogeneratedDisclaimer(OutFile &out)
{
	out()
			<< "// ----------------------------------------------------------------------- //\n"
			<< "// This file is auto-generated with qrxc v1.1. Do not modify its contents\n"
			<< "// or prepare to lose your edits. If you want to change something in it\n"
			<< "// consider editing language metamodel instead.\n"
			<< "// ----------------------------------------------------------------------- //\n\n";
}

void XmlCompiler::generateIncludes(OutFile &out)
{
	out() << "#include \"" << "pluginInterface.h\"\n" //mPluginName
		<< "\n";

	out() << "#include \"" << "elements.h" << "\"\n";

	out() << "\n";

	out()
		//<< "Q_EXPORT_PLUGIN2(qreal_editors, " << mPluginName << "Plugin)\n\n"
		<< mPluginName << "Plugin::" << mPluginName << "Plugin()\n"
		<< "\t:Metamodel(\"" << mPluginName << "\")"
		<< "{\n"
		<< "\tinitPlugin();\n"
		<< "}\n\n";
}

void XmlCompiler::generateInitPlugin(OutFile &out)
{
	out() << "void " << mPluginName << "Plugin::initPlugin()\n{\n"
		<< "\tsetVersion(\"" << mPluginVersion << "\");\n"
		<< "\n"
		<< "\tinitMultigraph();\n"
		<< "\tinitNameMap();\n"
		<< "\tinitPaletteGroupsMap();\n"
		<< "\tinitPaletteGroupsDescriptionMap();\n"
		<< "\tinitShallPaletteBeSortedMap();\n"
		<< "}\n\n";

	generateInitMultigraph(out);
	generateNameMappings(out);
	generatePaletteGroupsLists(out);
	generatePaletteGroupsDescriptions(out);
	generateShallPaletteBeSorted(out);
}

void XmlCompiler::generateInitMultigraph(OutFile &out)
{
	out() << "void " << mPluginName << "Plugin::initMultigraph()\n{\n";

	for (const Diagram *diagram : mEditors[mCurrentEditor]->diagrams()) {
		for (const Type *type : diagram->types()) {
			if (dynamic_cast<const GraphicType *>(type)) {
				const QString elementType = NameNormalizer::normalize(type->qualifiedName());
				out() << "\taddNode(*(new " << elementType << "(*this)));\n";
			}
		}
	}

	for (const Diagram *diagram : mEditors[mCurrentEditor]->diagrams()) {
		for (const Type *type : diagram->types()) {
			if (const GraphicType *graphicType = dynamic_cast<const GraphicType *>(type)) {
				generateLinks(out, graphicType, graphicType->immediateParents(), "generalizationLinkType", false);
				generateLinks(out, graphicType, graphicType->containedTypes(), "containmentLinkType", true);
				generateExplosionsMappings(out, graphicType);
			}
		}
	}

	out() << "}\n\n";
}

void XmlCompiler::generateLinks(OutFile &out, const Type *from, const QStringList &to
		, const QString &linkType, bool areNamesNormalized)
{
	for (const QString &toTypeName : to) {
		const Type *toType = areNamesNormalized
				? mEditors[mCurrentEditor]->findTypeByNormalizedName(toTypeName)
				: mEditors[mCurrentEditor]->findType(from->diagram()->name() + "::" + toTypeName);
		if (!toType || !mEditors[mCurrentEditor]->diagrams().contains(toType->diagram()->name())) {
			qWarning() << "Omitting generation of" << toTypeName << "because it is in imported module.";
			// Ignoring imported types.
			continue;
		}

		const QString fromDiagramName = NameNormalizer::normalize(from->diagram()->name());
		const QString toDiagramName = NameNormalizer::normalize(toType->diagram()->name());
		const QString fromName = NameNormalizer::normalize(from->qualifiedName());
		const QString toName = NameNormalizer::normalize(toType->qualifiedName());
		out() << QString("\tproduceEdge(elementType(\"%1\", \"%2\")"
				", elementType(\"%3\", \"%4\"), qReal::ElementType::%5);\n")
				.arg(fromDiagramName, fromName, toDiagramName, toName, linkType);
	}
}

void XmlCompiler::generateNameMappings(OutFile &out)
{
	out() << "void " << mPluginName << "Plugin::initNameMap()\n{\n";

	for (const Diagram * const diagram : mEditors[mCurrentEditor]->diagrams().values()) {
		const QString diagramName = NameNormalizer::normalize(diagram->name());
		const QString nodeName = NameNormalizer::normalize(diagram->nodeName());
		out() << "\taddDiagram(\"" << diagramName << "\");\n";
		out() << "\tsetDiagramFriendlyName(\"" << diagramName << "\", QObject::tr(\""
				<< diagram->displayedName() << "\"));\n";
		out() << "\tsetDiagramNode(\"" << diagramName << "\", \"" << nodeName << "\");\n";
		out() << "\n";
	}

	out() << "}\n\n";
}

void XmlCompiler::generatePaletteGroupsLists(utils::OutFile &out)
{
	out() << "void " << mPluginName << "Plugin::initPaletteGroupsMap()\n{\n";

	for (const Diagram * const diagram : mEditors[mCurrentEditor]->diagrams().values()) {
		const QString diagramName = NameNormalizer::normalize(diagram->name());
		const QList<QPair<QString, QStringList>> paletteGroups = diagram->paletteGroups();
		for (const QPair<QString, QStringList> &group : paletteGroups) {
			const QString groupName = group.first;
			const QString groupParameters = QString("\"%1\", QObject::tr(\"%2\")").arg(diagramName, groupName);
			out() << "\tappendDiagramPaletteGroup(" << groupParameters << ");\n";

			for (const QString &name : group.second) {
				out() << "\taddElementToDiagramPaletteGroup(" << groupParameters << ", QString::fromUtf8(\""
						<< NameNormalizer::normalize(name) << "\"));\n";
			}
		}
	}
	out() << "}\n\n";
}

void XmlCompiler::generatePaletteGroupsDescriptions(utils::OutFile &out)
{
	out() << "void " << mPluginName << "Plugin::initPaletteGroupsDescriptionMap()\n{\n";

	for (const Diagram * const diagram : mEditors[mCurrentEditor]->diagrams().values()) {
		const QString diagramName = NameNormalizer::normalize(diagram->name());
		const QMap<QString, QString> paletteGroupsDescriptions = diagram->paletteGroupsDescriptions();
		for (const QString &groupName : paletteGroupsDescriptions.keys()) {
			const QString groupParameters = QString("\"%1\", QObject::tr(\"%2\")").arg(diagramName, groupName);
			const QString descriptionName = paletteGroupsDescriptions[groupName];
			if (!descriptionName.isEmpty()) {
				out() << "\tsetDiagramPaletteGroupDescription(" << groupParameters << ", QObject::tr(\""
						<< descriptionName << "\"));\n";
			}
		}
	}

	out() << "}\n\n";
}

void XmlCompiler::generateShallPaletteBeSorted(utils::OutFile &out)
{
	out() << "void " << mPluginName << "Plugin::initShallPaletteBeSortedMap()\n{\n";

	for (const Diagram * const diagram : mEditors[mCurrentEditor]->diagrams().values()) {
		const QString diagramName = NameNormalizer::normalize(diagram->name());
		out() << "\tsetPaletteSorted(QString::fromUtf8(\""
				<< diagramName << "\"), " << (diagram->shallPaletteBeSorted() ? "true" : "false") << ");\n";
	}

	out() << "}\n\n";
}

void XmlCompiler::generateExplosionsMappings(OutFile &out, const GraphicType *graphicType)
{
	const QMap<QString, QPair<bool, bool>> &explosions = graphicType->explosions();
	for (const QString &target : explosions.keys()) {
		out() << QString("\taddExplosion(elementType(\"%1\", \"%2\"), elementType(\"%3\", \"%4\"), %5, %6);\n").arg(
				graphicType->diagram()->name(), graphicType->name()
				, graphicType->diagram()->name(), target
				, explosions[target].first ? "true" : "false"
				, explosions[target].second ? "true" : "false");
	}
}

void XmlCompiler::generateResourceFile()
{
	OutFile out("plugin.qrc");
	out() << mResources
		<< "</qresource>\n"
		<< "</RCC>\n";
}

void XmlCompiler::generateEnumValues(OutFile &out)
{
	out() << "void " << mPluginName << "Plugin::initEnums() \n{\n";

	for (const EnumType * const type : mEditors[mCurrentEditor]->getAllEnumTypes()) {
		const QString name = NameNormalizer::normalize(type->name());
		out() << "\taddEnum(\"" << name << "\", { ";
		QStringList pairs;
		const QMap<QString, QString> values = type->values();
		for (const QString &name : values.keys()) {
			pairs << QString("qMakePair(QString(\"%1\"), tr(\"%2\"))").arg(name, values[name]);
		}

		out() << pairs.join(", ");
		out() << " });\n";

		out() << "\tsetEnumEditable(\"" << name << "\", " << (type->isEditable() ? "true" : "false") << ");\n";
	}

	out() << "}\n\n";
}
