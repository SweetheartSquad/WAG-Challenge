#pragma once

#include <node\Node.h>
#include <json\json.h>

#include <NodeContent.h>
#include <Triggers.h>
#include <Conditions.h>

class Dialogue abstract : public NodeContent{
public:
	std::string speaker;
	std::string portrait;
	std::vector<std::string> text;
	signed long int currentText;
	std::vector<Trigger *> triggers;
	std::vector<Condition *> conditions;

	Dialogue(Json::Value _json);
	
	std::string getCurrentText();
	// increments currentText
	// returns true if there is a corresponding text to say
	// returns false if the end of the dialogue has been reached
	bool sayNextText();

	static Dialogue * getDialogue(Json::Value _json);
};

class DialogueSay : public Dialogue{
public:
	DialogueSay(Json::Value _json);
};


// a dialog object which prompts for user selection
class DialogueAsk : public Dialogue{
public:
	std::vector<std::string> options;
	std::vector<std::vector<Trigger *>> optionsResults;

	DialogueAsk(Json::Value _json);
	~DialogueAsk();
};