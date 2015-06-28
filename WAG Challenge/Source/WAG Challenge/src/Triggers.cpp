#pragma once

#include <Triggers.h>
#include <Log.h>
#include <WAG_ResourceManager.h>

Trigger * Trigger::getTrigger(Json::Value _json){
	Trigger * res = nullptr;
	std::string type = _json.get("type", "NO_TYPE").asString();
	// create a different type of Trigger depending on the value of type
	if(type == "setVar"){
		res = new TriggerSetVar(_json);
	}else if(type == "setConversation"){
		res = new TriggerSetConversation(_json);
	}else{
		throw "invalid trigger type";
	}
	return res;
}

TriggerSetVar::TriggerSetVar(Json::Value _json) :
	target(_json.get("target", "NO_TARGET").asString()),
	variable(_json.get("variable", "NO_VARIABLE").asString()),
	newValue(_json.get("value", "NO_NEW_VALUE").asString())
{
}

void TriggerSetVar::trigger(){
	Log::info("Set Var: " + variable + " -> " + newValue);
	WAG_ResourceManager::playthrough->characters.at(target)->variables.at(variable) = newValue;
}

TriggerSetConversation::TriggerSetConversation(Json::Value _json) :
	newConversation(_json.get("conversation", "NO_NEW_CONVERSATION").asString())
{
}

void TriggerSetConversation::trigger(){
	Log::info("New Conversation: " + newConversation);
	WAG_ResourceManager::playthrough->currentConversation = WAG_ResourceManager::playthrough->conversations[newConversation];
	WAG_ResourceManager::playthrough->currentConversation->reset();
}
