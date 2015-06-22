#pragma once

#include <Conversation.h>

Conversation::Conversation(Json::Value _json) :
	id(_json.get("id", "NO_ID").asString())
{
	Json::Value convoDialogueJson = _json["dialogue"];

	for(auto j = 0; j < convoDialogueJson.size(); ++j) {
		Json::Value sayAction = convoDialogueJson[j];
		if(sayAction.isMember("options")) {
			dialogueObjects.push_back(new DialogueAsk(sayAction));
		}else {
			dialogueObjects.push_back(new DialogueSay(sayAction));
		}
	}
}