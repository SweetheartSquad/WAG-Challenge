#pragma once

#include <Conversation.h>
#include <Log.h>
#include <assert.h>

Conversation::Conversation(Json::Value _json) :
	id(_json.get("id", "NO_ID").asString()),
	currentDialogue(0)
{
	Json::Value convoDialogueJson = _json["dialogue"];

	for(auto j = 0; j < convoDialogueJson.size(); ++j){
		dialogueObjects.push_back(Dialogue::getDialogue(convoDialogueJson[j]));
	}
}

Dialogue * Conversation::getCurrentDialogue(){
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


	// go to next dialogue
	// check that we still have something to say
	// check if we are allowed to say the current selection
	bool valid = false;
	while(currentDialogue < dialogueObjects.size()-1 && !valid){
		// if any conditions are untrue for a given dialogue object, skip over it
		valid = dialogueObjects.at(++currentDialogue)->evaluateConditions();
	}
	if(valid){
		// get the first text in the new dialogue object
		return dialogueObjects.at(currentDialogue)->sayNextText();
	}else{
		// no valid dialogue objects were found past this one
		return false;
	}
}

void Conversation::reset(){
	Log::info("Reset conversation: " + id);
	for(Dialogue * d : dialogueObjects){
		d->reset();
	}

	currentDialogue = 0;
	while(!dialogueObjects.at(currentDialogue)->evaluateConditions()){
		++currentDialogue;
		assert(currentDialogue < dialogueObjects.size());
	}
}