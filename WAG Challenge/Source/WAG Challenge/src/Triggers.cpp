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
	}else if(type == "addVar"){
		res = new TriggerAddVar(_json);
	}else if(type == "postData"){
		res = new TriggerPostData(_json);
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

void TriggerAddVar::trigger(){
	WAG_ResourceManager::playthrough->characters.at(target)->variables.at(variable) += num;
	Log::info("Add " + num + " to var: " + target + "->" + variable + " = " + WAG_ResourceManager::playthrough->characters.at(target)->variables.at(variable));
}

TriggerAddVar::TriggerAddVar(Json::Value _json) :
	target(_json.get("target", "NO_TARGET").asString()),
	variable(_json.get("variable", "NO_VARIABLE").asString()),
	num(_json.get("num", "NO_NUM").asString())
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













#include <thread>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/json.h>
// Retrieves a JSON value from an HTTP request.
pplx::task<void> RequestJSONValueAsync(std::wstring & _url){
	// TODO: To successfully use this example, you must perform the request  
	// against a server that provides JSON data.
	web::http::client::http_client client(_url);
	return client.request(web::http::methods::POST).then([](web::http::http_response response) -> pplx::task<web::json::value>{
		std::wcout << L"Response recieved" << std::endl << L"Status: " << response.status_code() << std::endl;
		if(response.status_code() == web::http::status_codes::OK){
			auto json = response.extract_json();
			return json;
		}else{
			std::wcout << L"No response because the code wasn't ok." << std::endl;
		}

		// Handle error cases, for now return empty json value... 
		return pplx::task_from_result(web::json::value());
	}).then([](pplx::task<web::json::value> previousTask){
		try{
			// Perform actions here to process the JSON value...
			const web::json::value & v = previousTask.get();
			web::json::array arr = v.as_array();

			for(auto i = 0; i < v.as_array().size(); ++i) {
				web::json::value obj = v.at(i);
				std::wstring name = obj.as_object().at(L"name").as_string();
				int eat =  obj.as_object().at(L"eat").as_integer();
				int no_eat =  obj.as_object().at(L"no_eat").as_integer();
				if(name == L"mayor") {
					WAG_ResourceManager::eatCountMayor = eat;
					WAG_ResourceManager::noEatCountMayor = no_eat;
				}else if(name == L"chipmunk") {
					WAG_ResourceManager::eatCountChipmunk = eat;
					WAG_ResourceManager::noEatCountChipmunk = no_eat;
				}else if(name == L"marten") {
					WAG_ResourceManager::eatCountMartent = eat;
					WAG_ResourceManager::noEatCountMarten = no_eat;
				}else if(name == L"squirrel") {
					WAG_ResourceManager::eatCountSquirrel = eat;
					WAG_ResourceManager::noEatCountSquirrel = no_eat;
				}else if(name == L"dormouse") {
					WAG_ResourceManager::eatCountDormouse = eat;
					WAG_ResourceManager::noEatCountDormouse = no_eat;
				}
			}
			/*std::wcout << L"Test: " << v << std::endl;
			
			WAG_ResourceManager::noEatCount = std::stoi(v.as_object().at(L"noEat").as_string());
			std::stringstream ss;
			ss << "Eat: " << WAG_ResourceManager::eatCount << ", No Eat: " << WAG_ResourceManager::noEatCount;
			Log::info(ss.str());*/

			std::cout << WAG_ResourceManager::eatCountMayor << std::endl;
			std::cout << WAG_ResourceManager::noEatCountMayor << std::endl;
			std::cout << WAG_ResourceManager::eatCountChipmunk << std::endl;
			std::cout << WAG_ResourceManager::noEatCountChipmunk << std::endl;
			std::cout << WAG_ResourceManager::eatCountMartent << std::endl;
			std::cout << WAG_ResourceManager::noEatCountMarten << std::endl;
			std::cout << WAG_ResourceManager::eatCountSquirrel << std::endl;
			std::cout << WAG_ResourceManager::noEatCountSquirrel << std::endl;
			std::cout << WAG_ResourceManager::eatCountDormouse << std::endl;
			std::cout << WAG_ResourceManager::noEatCountDormouse << std::endl;

		}catch (const web::http::http_exception& e){
			// Print error.
			std::wostringstream ss;
			ss << e.what() << std::endl;
			std::wcout << ss.str();
		}
	});
}

TriggerPostData::TriggerPostData(Json::Value _json) :
	url(_json.get("url", "NO_URL").asString())
{
	
	const Json::Value vObj = _json["variables"];
	const Json::Value::Members vMembers = vObj.getMemberNames();
	for(auto v : vMembers){
		variables[v] = vObj[v].asString();
	}
}

void TriggerPostData::trigger(){

	std::wstringstream ss;
	ss << std::wstring(url.begin(), url.end()) << L"?";
	for(auto i : variables){
		ss << std::wstring(i.first.begin(), i.first.end()) << L"=" << std::wstring(i.second.begin(), i.second.end()) << L"&";
	}
	ss << L"&source=WAG";
	Log::info("POSTing data to: " + url);

	RequestJSONValueAsync(ss.str()).wait();
}

