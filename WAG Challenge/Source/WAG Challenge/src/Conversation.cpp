#pragma once

#include <Conversation.h>

Conversation::Conversation(Json::Value _json) :
	id(_json.get("id", "NO_ID").asString()),
	currentDialogue(0)
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

DialogueSay * Conversation::getCurrentDialogue(){
	return dialogueObjects.at(currentDialogue);
}

bool Conversation::sayNextDialogue(){
	// if there aren't any dialogue objects left, the conversation is over so return false
	if(currentDialogue >= dialogueObjects.size()){
		return false;
	}

	// if there's still text in the current dialogue object,
	// we don't increment and instead return true to indicate
	// that the conversation is still going
	if(dialogueObjects.at(currentDialogue)->sayNextText()){
		return true;
	}
	++currentDialogue;
	
	// if there aren't any dialogue objects left, the conversation is over so return false
	if(currentDialogue >= dialogueObjects.size()){
		return false;
	}
	// if any conditions are untrue for a given dialogue object, skip over it
	for(Condition * c : dialogueObjects.at(currentDialogue)->conditions){
		if(!c->evaluate()){
			++currentDialogue;
			return sayNextDialogue();
		}
	}

	// get the first text in the new dialogue object
	return dialogueObjects.at(currentDialogue)->sayNextText();
}