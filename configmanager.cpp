#include "configmanager.h"
json ConfigManager::OpenConfigFile()
{
    std::ifstream f(m_config_file_path.toStdString());
    json data = NULL;
    if (f.is_open())
    {
        data = json::parse(f);
    }
    return data;
}

void ConfigManager::SaveConfigFile()
{
    std::ofstream f(m_config_file_path.toStdString());
    ConfigData d;
    json j;
    m_data->to_json(j);
    f << j.dump(4);
    f.close();
}
