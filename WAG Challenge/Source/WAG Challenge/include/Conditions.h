#pragma once

#include <node\Node.h>
#include <json\json.h>

#include <NodeContent.h>

class Condition abstract: public NodeContent{
public:
	static Condition * getCondition(Json::Value _json);
	
	std::string target;
	std::string variable;

	Condition(Json::Value _json);

	// returns true if the condition's criteria are met
	virtual bool evaluate() = 0;
};

class ConditionEquality : public Condition{
public:
	std::string requiredValue;

	ConditionEquality(Json::Value _json);
	virtual bool evaluate() override;
};
