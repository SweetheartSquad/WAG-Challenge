#pragma once

#include <node\Node.h>
#include <json\json.h>

#include <functional>
#include <NodeContent.h>

class Trigger abstract : public NodeContent{
public:
	static Trigger * getTrigger(Json::Value _json);
	
	virtual void trigger() = 0;
};

class TriggerSetVar : public Trigger{
public:
	std::string target;
	std::string variable;
	std::string newValue;

	TriggerSetVar(Json::Value _json);
	virtual void trigger() override;
};

class TriggerSetConversation : public Trigger{
public:
	std::string newConversation;

	TriggerSetConversation(Json::Value _json);
	virtual void trigger() override;
};