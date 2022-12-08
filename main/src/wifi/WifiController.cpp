#include "WifiController.h"

namespace RestApi
{
	void scanWifi()
	{
		log_i("scanWifi");
		int networkcount = WiFi.scanNetworks();
		if (networkcount == -1)
		{
			while (true)
				;
		}
		DynamicJsonDocument doc(4096);
		for (int i = 0; i < networkcount; i++)
		{
			doc.add(WiFi.SSID(i));
		}
		serialize(doc);
	}

	void connectToWifi()
	{
		serialize(WifiController::connect(deserialize()));
	}
}

namespace WifiController
{

	WebServer *server = nullptr;
	WebSocketsServer *webSocket = nullptr;
	WifiConfig *config;

	String getSsid()
	{
		return config->mSSID;
	}
	String getPw()
	{
		return config->mPWD;
	}
	bool getAp()
	{
		return config->mAP;
	}

	WebServer *getServer()
	{
		return server;
	}

	void handelMessages()
	{
		if (webSocket != nullptr)
			webSocket->loop();
		if (server != nullptr)
			server->handleClient();
	}

	DynamicJsonDocument connect(DynamicJsonDocument doc)
	{
		log_i("connectToWifi");
		
		bool ap = doc[keyWifiAP];
		String ssid = doc[keyWifiSSID];
		String pw = doc[keyWifiPW];
		log_i("ssid: %s wifi:%s", ssid.c_str(), WifiController::getSsid().c_str());
		log_i("pw: %s wifi:%s", pw.c_str(), WifiController::getPw().c_str());
		log_i("ap: %s wifi:%s", ap, WifiController::getAp());
		WifiController::setWifiConfig(ssid, pw, ap);
		log_i("ssid json: %s wifi:%s", ssid, WifiController::getSsid());
		log_i("pw json: %s wifi:%s", pw, WifiController::getPw());
		log_i("ap json: %s wifi:%s", ap, WifiController::getAp());
		WifiController::setup();
		WifiController::begin();
		doc.clear();
		return doc;
	}

	void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
	{
		if(type == WStype_DISCONNECTED)
			log_i("[%u] Disconnected!\n", num);
		else if(type == WStype_CONNECTED)
		{
			IPAddress ip = webSocket->remoteIP(num);
			log_i("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
		}
		else if(type == WStype_TEXT)
		{
			log_i("[%u] get Text: %s\n", num, payload);
			DynamicJsonDocument doc(4096);
			deserializeJson(doc, payload);
			if (doc.containsKey(keyLed) && moduleController.get(AvailableModules::led) != nullptr)
				moduleController.get(AvailableModules::led)->act(doc);
			if (doc.containsKey(key_motor) && moduleController.get(AvailableModules::motor) != nullptr)
				moduleController.get(AvailableModules::motor)->act(doc);
		}
	}

	void sendJsonWebSocketMsg(DynamicJsonDocument doc)
	{
		//log_i("socket broadcast");
		String s;
		serializeJson(doc, s);
		webSocket->broadcastTXT(s.c_str());
	}

	void setWifiConfig(String SSID, String PWD, bool ap)
	{
		log_i("mssid:%s pw:%s ap:%s", config->mSSID, config->mPWD, config->mAP);
		config->mSSID = SSID;
		config->mPWD = PWD;
		config->mAP = ap;
		Config::setWifiConfig(config);
	}

	void createAp(String ssid, String password)
	{
		WiFi.disconnect();
		log_i("Ssid %s pw %s", ssid, password);

		if (ssid.isEmpty())
		{
			log_i("Ssid empty, start Uc2 open softap");
			WiFi.softAP(config->mSSIDAP.c_str());
		}
		else if (password.isEmpty())
		{
			log_i("Ssid start %s open softap", ssid);
			WiFi.softAP(ssid.c_str());
		}
		else
		{
			log_i("Ssid start %s close softap", ssid);
			WiFi.softAP(ssid.c_str(), password.c_str());
		}
		log_i("Connected. IP: %s", WiFi.softAPIP().toString());
	}

	void setup()
	{
		config = Config::getWifiConfig();
		if (config->mSSID != nullptr)
			log_i("mssid:%s pw:%s ap:%s", config->mSSID, config->mPWD, config->mAP);
		if (server != nullptr)
			server->close();
		if (webSocket != nullptr)
			webSocket->close();
		if (config->mSSID == "")
		{
			config->mAP = true;
			createAp(config->mSSIDAP, config->mPWD);
		}
		else if (config->mAP)
		{
			createAp(config->mSSID, config->mPWD);
		}
		else
		{
			WiFi.softAPdisconnect();
			log_i("Connect to:%s", config->mSSID);
			WiFi.begin(config->mSSID.c_str(), config->mPWD.c_str());

			int nConnectTrials = 0;
			while (WiFi.status() != WL_CONNECTED && nConnectTrials <= 5)
			{
				log_i("Wait for connection");
				delay(400);
				nConnectTrials++;
			}
			if (nConnectTrials >= 5)
			{
				log_i("failed to connect,Start softap");
				config->mAP = true;
				config->mSSID = config->mSSIDAP;
				config->mPWD = "";
				createAp(config->mSSIDAP, "");
			}
			else
			{
				log_i("Connected. IP: %s", WiFi.localIP());
			}
		}
		if (server == nullptr)
			server = new WebServer(80);
		if (webSocket == nullptr)
			webSocket = new WebSocketsServer(81);
	}

	void restartWebServer()
	{
		if (server != nullptr)
		{
			server->close();
		}
		setup_routing();
		server->begin();
	}

	void begin()
	{
		webSocket->begin();
		webSocket->onEvent(webSocketEvent);
		setup_routing();
		server->begin();
	}

	void getIndexPage()
	{
		SPIFFS.begin();
		File file = SPIFFS.open("/index.html", "r");
		server->streamFile(file, "text/html");
		file.close();
		SPIFFS.end();
	}

	void getjquery()
	{
		SPIFFS.begin();
		File file = SPIFFS.open("/jquery-3.6.1.min.js", "r");
		server->streamFile(file, "text/javascript");
		file.close();
		SPIFFS.end();
	}
	void getjs()
	{
		SPIFFS.begin();
		File file = SPIFFS.open("/script.js", "r");
		server->streamFile(file, "text/javascript");
		file.close();
		SPIFFS.end();
	}

	void getCSS()
	{
		SPIFFS.begin();
		File file = SPIFFS.open("/styles.css", "r");
		server->streamFile(file, "text/css");
		file.close();
		SPIFFS.end();
	}

	void getOtaIndex()
	{
		SPIFFS.begin();
		File file = SPIFFS.open("/ota.html", "r");
		server->streamFile(file, "text/html");
		file.close();
		SPIFFS.end();
	}

	void setup_routing()
	{
		log_i("Setting up HTTP Routing");
		server->onNotFound(RestApi::handleNotFound);

		server->on(features_endpoint, RestApi::getEndpoints);

		server->on(ota_endpoint, HTTP_GET, getOtaIndex);
		/*handling uploading firmware file */
		server->on(update_endpoint, HTTP_POST, RestApi::update, RestApi::upload);

		server->on("/", HTTP_GET, getIndexPage);
		server->on("/styles.css", HTTP_GET, getCSS);
		server->on("/script.js", HTTP_GET, getjs);
		server->on("/jquery-3.6.1.min.js", HTTP_GET, getjquery);
		server->on(scanwifi_endpoint, HTTP_GET, RestApi::scanWifi);
		server->on(connectwifi_endpoint, HTTP_POST, RestApi::connectToWifi);
		server->on(reset_nv_flash_endpoint, HTTP_GET, RestApi::resetNvFLash);

		server->on(bt_scan_endpoint, HTTP_GET, RestApi::Bt_startScan);
		server->on(bt_connect_endpoint, HTTP_POST, RestApi::Bt_connect);
		server->on(bt_remove_endpoint, HTTP_POST, RestApi::Bt_remove);
		server->on(bt_paireddevices_endpoint, HTTP_GET, RestApi::Bt_getPairedDevices);

		server->on(modules_get_endpoint, HTTP_GET, RestApi::getModules);
		server->on(modules_set_endpoint, HTTP_POST, RestApi::setModules);

		// POST
		if (moduleController.moduleConfig->motor != 0)
		{
			log_i("add motor endpoints");
			server->on(motor_act_endpoint, HTTP_POST, RestApi::FocusMotor_act);
			server->on(motor_get_endpoint, HTTP_GET, RestApi::FocusMotor_get);
			server->on(motor_set_endpoint, HTTP_POST, RestApi::FocusMotor_set);
			server->on(motor_setcalibration_endpoint,HTTP_POST, RestApi::FocusMotor_setCalibration);
		}

		if (moduleController.moduleConfig->led != 0)
		{
			log_i("add led endpoints");
			server->on(ledarr_act_endpoint, HTTP_POST, RestApi::Led_act);
			server->on(ledarr_get_endpoint, HTTP_GET, RestApi::Led_get);
			server->on(ledarr_set_endpoint, HTTP_POST, RestApi::Led_set);
		}

		if (moduleController.moduleConfig->analogJoystick != 0)
		{
			log_i("add analog joystick endpoints");
			server->on(analog_joystick_set_endpoint, HTTP_POST, RestApi::AnalogJoystick_set);
			server->on(analog_joystick_get_endpoint, HTTP_POST, RestApi::AnalogJoystick_get);
		}
		log_i("Setting up HTTP Routing END");
	}
}
