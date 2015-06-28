#pragma once

#include <Conditions.h>
#include <WAG_ResourceManager.h>
#include <Log.h>

Condition * Condition::getCondition(Json::Value _json){
	Condition * res = nullptr;
	std::string type = _json.get("type", "NO_TYPE").asString();
	// create a different type of Trigger depending on the value of type
	if(type == "equality"){
		res = new ConditionEquality(_json);
	}else{
		throw "invalid trigger type";
	}
	return res;
}

Condition::Condition(Json::Value _json) :
	target(_json.get("target", "NO_TARGET").asString()),
	variable(_json.get("variable", "NO_VARIABLE").asString())
{
}

ConditionEquality::ConditionEquality(Json::Value _json) :
	Condition(_json),
	requiredValue(_json.get("value", "NO_REQUIRED_VALUE").asString())
{
}

bool ConditionEquality::evaluate(){
	bool res = WAG_ResourceManager::playthrough->characters.at(target)->variables.at(variable) == requiredValue;
	Log::info((res ? "Condition passed: " : "Condition failed: ") + target + "->" + variable + "("+WAG_ResourceManager::playthrough->characters.at(target)->variables.at(variable)+") == " + requiredValue);
	return res;
}