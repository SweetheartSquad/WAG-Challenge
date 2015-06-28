#pragma once

#include <NodeContent.h>
#include <Dialogue.h>

class Conversation : public NodeContent{
private:
	// keeps track of the progress within this conversation
	unsigned long int currentDialogue;

public:
	std::string id;
	std::vector<Dialogue *> dialogueObjects;
	Conversation(Json::Value _json);

	// returns dialogueObjects.at(currentDialogue)
	Dialogue * getCurrentDialogue();
	// increments currentDialogue
	// returns true if there is a corresponding dialogue object
	// returns false if the end of the conversation has been reached
	bool sayNextDialogue();
};