#pragma once

#include <WAG_SceneMain.h>
#include <WAG_Game.h>
#include <WAG_ResourceManager.h>

#include <MeshEntity.h>
#include <MeshInterface.h>
#include <MeshFactory.h>
#include <Resource.h>

#include <shader\ComponentShaderBase.h>
#include <shader\ShaderComponentTexture.h>
#include <shader\ShaderComponentDiffuse.h>
#include <shader\ShaderComponentHsv.h>

#include <System.h>
#include <GLFW\glfw3.h>

#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <NodeBulletBody.h>
#include <BulletMeshEntity.h>

#include <OpenALSound.h>

#include <thread>

#include <DialogueDisplay.h>

// Retrieves a JSON value from an HTTP request.
pplx::task<void> RequestJSONValueAsync(TextLabel * _label){
	// TODO: To successfully use this example, you must perform the request  
	// against a server that provides JSON data.
	web::http::client::http_client client(L"https://seniorproject-ryanbluth.c9.io/api/users");
	return client.request(web::http::methods::GET).then([_label](web::http::http_response response) -> pplx::task<web::json::value>{
		std::wcout << L"Response recieved" << std::endl << L"Status: " << response.status_code() << std::endl;
		if(response.status_code() == web::http::status_codes::OK){
			auto json = response.extract_json();

			_label->setText(json.get()[0].at(L"user").at(L"email").as_string());
            return json;
		}else{
			std::wcout << L"No response because the code wasn't ok." << std::endl;
		}

		// Handle error cases, for now return empty json value... 
		return pplx::task_from_result(web::json::value());
	}).then([](pplx::task<web::json::value> previousTask){
		try{
			const web::json::value& v = previousTask.get();
			// Perform actions here to process the JSON value...
		}catch (const web::http::http_exception& e){
			// Print error.
			std::wostringstream ss;
			ss << e.what() << std::endl;
			std::wcout << ss.str();
		}
	});
}

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
	int i;
	for(i=0; i<argc; i++){
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

WAG_SceneMain::WAG_SceneMain(Game * _game) :
	WAG_Scene(_game),
	dialogueDisplay(new DialogueDisplay(uiLayer->world, this, font, textShader, 1.f, 1.f))
{
	uiLayer->addChild(dialogueDisplay);

	// start the game proper
	WAG_ResourceManager::playthrough->currentConversation = WAG_ResourceManager::playthrough->conversations["MENU"];
	WAG_ResourceManager::playthrough->currentConversation->reset();
	Step step;
	dialogueDisplay->update(&step);
	
	// background music
	WAG_ResourceManager::playthrough->getAudio("bgm")->sound->play(true);
}

void WAG_SceneMain::update(Step * _step){
	WAG_ResourceManager::playthrough->getAudio("bgm")->sound->update(_step);
	
	// skip button
	if(keyboard->keyDown(GLFW_KEY_SPACE) && WAG_ResourceManager::skipEnabled){
		dialogueDisplay->dialogue->finishTicking();
		dialogueDisplay->autoProgress = true;
		dialogueDisplay->autoProgressTimer->targetSeconds = 0.01f;
	}else{
		dialogueDisplay->autoProgress = false;
	}

	if(keyboard->keyJustUp(GLFW_KEY_E)){	
		std::wcout << L"Calling RequestJSONValueAsync..." << std::endl;
		//RequestJSONValueAsync(label);
	}
	if(keyboard->keyJustUp(GLFW_KEY_ESCAPE)){	
		game->switchScene("MENU", false);
	}

	WAG_Scene::update(_step);
}