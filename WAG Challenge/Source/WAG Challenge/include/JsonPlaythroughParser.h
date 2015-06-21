#pragma once

#include <json/json.h>
#include <vector>
#include <Conversation.h>

class JsonPlaythroughParser {
public:	

	Json::Value root;
	std::vector<Conversation *> conversations;

	JsonPlaythroughParser(std::string _jsonSrc);

};