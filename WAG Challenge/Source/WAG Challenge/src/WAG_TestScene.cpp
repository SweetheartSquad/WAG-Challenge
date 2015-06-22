#pragma once

#include <WAG_TestScene.h>
#include <WAG_Game.h>
#include <WAG_ResourceManager.h>

#include <MeshEntity.h>
#include <MeshInterface.h>
#include <MeshFactory.h>
#include <Resource.h>

#include <DirectionalLight.h>
#include <PointLight.h>
#include <Material.h>

#include <shader\ComponentShaderBase.h>
#include <shader\ShaderComponentTexture.h>
#include <shader\ShaderComponentDiffuse.h>
#include <shader\ShaderComponentPhong.h>
#include <shader\ShaderComponentBlinn.h>
#include <shader\ShaderComponentShadow.h>
#include <shader\ShaderComponentHsv.h>

#include <Box2DWorld.h>
#include <Box2DMeshEntity.h>
#include <Box2DDebugDrawer.h>

#include <MousePerspectiveCamera.h>
#include <FollowCamera.h>

#include <System.h>
#include <Mouse.h>
#include <Keyboard.h>
#include <GLFW\glfw3.h>
#include <MatrixStack.h>

#include <RenderSurface.h>
#include <StandardFrameBuffer.h>
#include <NumberUtils.h>
#include <RenderOptions.h>
#include <shader\ShaderComponentText.h>
#include <StringUtils.h>
#include <CharacterUtils.h>

#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <NodeBulletBody.h>
#include <BulletMeshEntity.h>
#include <Billboard.h>

#include <BulletRagdoll.h>
#include <NodeUI.h>
#include <WAG_Button.h>

#include <OpenALSound.h>
#include <sqlite\sqlite3.h>
#include <DatabaseConnection.h>

#include <Room.h>
#include <RoomLayout.h>

#include <thread>
#include <LinearLayout.h>
#include <sqlite\sqlite3.h>
#include <DatabaseConnection.h>

#include "RoomLayout.h"

#include <TextArea.h>
#include <shader\ComponentShaderText.h>
#include <DialogueDisplay.h>
#include <HorizontalLinearLayout.h>
#include <JsonPlaythroughParser.h>

// Retrieves a JSON value from an HTTP request.
pplx::task<void> RequestJSONValueAsync(Label * _label){
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

DatabaseConnection db("../assets/test.db");
void testSql(std::string _sql, bool _async){
	if(_async){
		db.queryDbAsync(_sql, callback);
	}else{
		db.queryDb(_sql, callback);
	}
}

WAG_TestScene::WAG_TestScene(Game * _game) :
	Scene(_game),
	shader(new ComponentShaderBase(true)),
	textShader(new ComponentShaderText(true)),
	screenSurfaceShader(new Shader("../assets/RenderSurface", false, true)),
	screenSurface(new RenderSurface(screenSurfaceShader)),
	screenFBO(new StandardFrameBuffer(true)),
	phongMat(new Material(15.0, glm::vec3(1.f, 1.f, 1.f), true)),
	sceneHeight(150),
	sceneWidth(50),
	joy(new JoystickManager()),
	uiLayer(this, 0,0,0,0)
{

	JsonPlaythroughParser * parser = new JsonPlaythroughParser("../assets/json_structure.json");

	shader->addComponent(new ShaderComponentTexture(shader));
	shader->compileShader();

	//Set up cameras
	screenSurface->scaleModeMag = GL_NEAREST;
	screenSurface->scaleModeMin = GL_NEAREST;

	font = new Font("../assets/fonts/Mathlete-Skinny.otf", 24, false);
	
	textShader->textComponent->setColor(glm::vec3(1.f, 1.f, 1.f));
	
	glm::uvec2 sd = vox::getScreenDimensions();
	uiLayer.resize(0, sd.x, 0, sd.y);

	srand(time(NULL));
	DialogueDisplay * dd = new DialogueDisplay(uiLayer.world, this, font, textShader, 1.f, 1.f);
	uiLayer.addChild(dd);
	//childTransform->addChild(dd);
	std::string test = "{ \"speaker\":\"cheryl\", \"portrait\":\"cheryl\", \"text\": [\"This is a dialogue thing\", \"beep\", \"boop\", \"i am a robot\"] }";
	std::string test2 = "{ \"speaker\":\"not cheryl\", \"portrait\":\"not implemented\", \"text\": [\"thanks cheryl\"] }";
	std::string test3 = "{ \"speaker\":\"this is an extra long name to see if it wraps or not\", \"portrait\":\"not implemented\", \"text\": [\"this text is also pretty long so that we can test out the text-wrapping stuff.\"] }";
	std::string test4 = "{ \"speaker\":\"cheryl\", \"portrait\":\"cheryl\", \"text\": [\"question???\"], \"options\": ["
		"{\"text\": \"option 1\", \"triggers\": [ {\"type\":\"stateChange\", \"target\":\"CHOICE\", \"newState\":\"you chose one\" } ] },"
		//"{\"text\": \"option 2\", \"triggers\": [ {\"type\":\"stateChange\", \"target\":\"CHOICE\", \"newState\":\"you chose two\" } ] },"
		"{\"text\": \"option 3\", \"triggers\": [ {\"type\":\"stateChange\", \"target\":\"CHOICE\", \"newState\":\"you chose three\" } ] } ] }";

	dd->stuffToSay.push_back(parser->conversations.at(0)->dialogueObjects.at(0));
	dd->stuffToSay.push_back(parser->conversations.at(0)->dialogueObjects.at(1));
	dd->stuffToSay.push_back(parser->conversations.at(0)->dialogueObjects.at(2));
	dd->stuffToSay.push_back(parser->conversations.at(0)->dialogueObjects.at(3));
	Step step;
	dd->update(&step);
	dd->sayNext();
	//dd->portraitPanel->mesh->pushTexture2D(WAG_ResourceManager::cheryl);
	//childTransform->addChild(dd);
	//dd->parents.at(0)->translate(300, 300, 0);

	mouseIndicator = new Sprite();
	uiLayer.childTransform->addChild(mouseIndicator);
	mouseIndicator->mesh->pushTexture2D(WAG_ResourceManager::cursor);
	mouseIndicator->parents.at(0)->scale(32, 32, 1);
	mouseIndicator->mesh->scaleModeMag = GL_NEAREST;
	mouseIndicator->mesh->scaleModeMin = GL_NEAREST;

	for(unsigned long int i = 0; i < mouseIndicator->mesh->vertices.size(); ++i){
		mouseIndicator->mesh->vertices[i].x += 0.5f;
		mouseIndicator->mesh->vertices[i].y -= 0.5f;
	}
	mouseIndicator->mesh->dirty = true;
	mouseIndicator->setShader(uiLayer.shader, true);
}

WAG_TestScene::~WAG_TestScene(){
	deleteChildTransform();
	shader->safeDelete();
	//delete phongMat;

	screenSurface->safeDelete();
	//screenSurfaceShader->safeDelete();
	screenFBO->safeDelete();
	delete joy;
	delete font;
}


void WAG_TestScene::update(Step * _step){
	WAG_ResourceManager::stream->update(_step);
	// handle inputs
	joy->update(_step);
	
	if(keyboard->keyJustUp(GLFW_KEY_P)){	
		WAG_ResourceManager::stream->play(true);
	}
	if(keyboard->keyJustUp(GLFW_KEY_E)){	
		std::wcout << L"Calling RequestJSONValueAsync..." << std::endl;
		//RequestJSONValueAsync(label);
	}
	if(keyboard->keyJustUp(GLFW_KEY_R)){	
		std::stringstream sql;
		sql << "DROP TABLE TestTable;";
		sql << "CREATE TABLE TestTable(id INTEGER PRIMARY KEY, TestColumn1, TestColumn2);";
		for(unsigned long int i = 0; i < 1000; ++i){
			sql << "INSERT INTO TestTable VALUES(" << i << ", 'test1', 'test2');";
		}
		sql << "SELECT * FROM TestTable;";
		testSql(sql.str(), true);
	}
	
	if(keyboard->keyJustUp(GLFW_KEY_2)){
		if(Transform::drawTransforms){
			uiLayer.bulletDebugDrawer->setDebugMode(btIDebugDraw::DBG_NoDebug);
		}else{
			uiLayer.bulletDebugDrawer->setDebugMode(btIDebugDraw::DBG_MAX_DEBUG_DRAW_MODE);
		}
		Transform::drawTransforms = !Transform::drawTransforms;
	}
	
	// update scene and physics
	Scene::update(_step);

	// update ui stuff
	glm::uvec2 sd = vox::getScreenDimensions();
	uiLayer.resize(0, sd.x, 0, sd.y);
	uiLayer.update(_step);

	mouseIndicator->parents.at(0)->translate(mouse->mouseX(), mouse->mouseY(), 0, false);
}

void WAG_TestScene::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	clear();
	uiLayer.render(_matrixStack, _renderOptions);
}

void WAG_TestScene::load(){
	Scene::load();	

	screenSurface->load();
	screenFBO->load();
	uiLayer.load();
}

void WAG_TestScene::unload(){
	uiLayer.unload();
	screenFBO->unload();
	screenSurface->unload();

	Scene::unload();	
}