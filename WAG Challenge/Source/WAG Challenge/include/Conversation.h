#pragma once

#include <NodeContent.h>
#include <DialogueSay.h>

class Conversation : public NodeContent{
public:

	std::vector<DialogueSay *> dialogueObjects;
	Conversation();
};