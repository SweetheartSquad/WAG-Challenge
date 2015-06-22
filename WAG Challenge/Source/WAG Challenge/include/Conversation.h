#pragma once

#include <NodeContent.h>
#include <DialogueSay.h>

class Conversation : public NodeContent{
public:
	std::string id;
	std::vector<DialogueSay *> dialogueObjects;
	Conversation(Json::Value _json);
};