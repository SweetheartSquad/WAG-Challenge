#pragma once

#include <JsonPlaythroughParser.h>
#include <Log.h>
#include <Conversation.h>
#include <DialogueSay.h>
#include <FileUtils.h>

Character::Character(Json::Value _json) :
	id(_json.get("id", "NO_ID").asString())
{
	const Json::Value vObj = _json["variables"];
	const Json::Value::Members vMembers = vObj.getMemberNames();
	for(auto v : vMembers){
		variables[v] = vObj[v].asString();
	}
}

JsonPlaythroughParser::JsonPlaythroughParser(std::string _jsonSrc) {
	std::string jsonLoaded = FileUtils::voxReadFile(_jsonSrc);
	Json::Reader reader;
	bool parsingSuccessful = reader.parse( jsonLoaded, root );
	if(!parsingSuccessful){
		Log::error("JSON parse failed: " + reader.getFormattedErrorMessages() + "\n" + jsonLoaded);
	}else{
		
		 Json::Value charactersJson = root["characters"];
		 for(auto i = 0; i < charactersJson.size(); ++i) {
			Character * c = new Character(charactersJson[i]);
			characters[c->id] = c;
		 }

		 Json::Value conversationsJson = root["conversations"];
		 for(auto i = 0; i < conversationsJson.size(); ++i) {
			
			Json::Value convoJson = conversationsJson[i];
			Json::Value convoDialogueJson = convoJson["dialogue"];
			
			Conversation * convo = new Conversation();
			
			for(auto j = 0; j < convoDialogueJson.size(); ++j) {
				Json::Value sayAction = convoDialogueJson[j];
				DialogueSay * say = new DialogueSay();
				if(sayAction.isMember("options")) {
					convo->dialogueObjects.push_back(new DialogueAsk(sayAction));
				}else {
					convo->dialogueObjects.push_back(new DialogueSay(sayAction));
				}
			}
			conversations.push_back(convo);
		 }
	}
}