#pragma once
#include <QString>
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;
struct ConfigData
{
	std::string version;
	std::string repoUrl;
	int scroll_speed;
	int scroll_interval;
	int pic_width;
	int pic_height;

	ConfigData()
	{
		version = "1.0.0";
		repoUrl = "git@github.com:CICSilver/PicMov.git";
		scroll_speed = 10;
		scroll_interval = 200;
		pic_width = 1080;
		pic_height = 1560;
	}

	void from_json(const json& data)
	{
		version = data["version"].get<std::string>();
		repoUrl = data["repoUrl"].get<std::string>();
		scroll_speed = data["scroll_speed"].get<int>();
		scroll_interval = data["scroll_interval"].get<int>();
		pic_width = data["pic_width"].get<int>();
		pic_height = data["pic_height"].get<int>();
	}

	void to_json(json &data)
	{
		data["version"] = version;
		data["repoUrl"] = repoUrl;
		data["scroll_speed"] = scroll_speed;
		data["scroll_interval"] = scroll_interval;
		data["pic_width"] = pic_width;
		data["pic_height"] = pic_height;
	}
};
class ConfigManager
{
public:
	static ConfigManager* Get()
	{
		static ConfigManager config_manager;
		return &config_manager;
	}

	void SaveConfigFile();

	ConfigData* GetConfigData() { return m_data; }

private:
	json OpenConfigFile();

	ConfigManager() 
	{
		m_config_file_path = "./config.json";
		m_data = new ConfigData();
		json data = OpenConfigFile();
		if (data != NULL)
		{
			m_data->from_json(data);
		}
	}
	~ConfigManager() {}

private:
	ConfigData* m_data;
	QString m_config_file_path;
};

