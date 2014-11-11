#include "generatorBase/semanticTree/rootNode.h"

#include "generatorBase/semanticTree/simpleNode.h"

using namespace generatorBase::semantics;
using namespace qReal;

RootNode::RootNode(Id const &initialBlock, QObject *parent)
	: SemanticNode(Id(), parent)
	, mZone(new ZoneNode(this))
	, mInitialBlock(initialBlock)
{
	mZone->setParentNode(this);
	mZone->appendChild(new SimpleNode(initialBlock, mZone));
}

Id RootNode::initialBlock() const
{
	return mInitialBlock;
}

QString RootNode::toStringImpl(GeneratorCustomizer &customizer, int indent) const
{
	return mZone->toString(customizer, indent);
}

QLinkedList<SemanticNode *> RootNode::children() const
{
	return QLinkedList<SemanticNode *>() << mZone;
}
