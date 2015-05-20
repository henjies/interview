#include "ini_parser.h"

namespace qh
{
	//ctor
	INIParser::INIParser()
	{
	}
	
	//dctor
	INIParser::~INIParser()
	{
	}

	//解析磁盘上的ini文件，默认最大容量为1024字节，默认section分隔符为[]
	//默认键值对分隔符为'\n',键和数值以'='分割
	//文件里面必须有至少一个section名字，不然提取不到键-值
	bool INIParser::Parse(const std::string &ini_file_path)
	{
		int fd;
		fd = open(ini_file_path.c_str(),O_RDONLY|O_EXCL);
		if ( fd == -1 )	
			return false;
		char buf[MAX_LEN];
		AllMap.clear();				//清空以前的历史记录
		ssize_t count = read(fd,buf,MAX_LEN);
		close(fd);

		if (count == -1)
			return false;

		ssize_t i=0;
		while (i < count && buf[i] != '[')//找到section第一个名字
			++i;
		if (i >= count)
			return false;

		while(i < count)
		{
			std::string sectionName;
			int index = GetMatchIndex(buf+i+1,']', sectionName, true);

			if (index == -1 || index == 0 || i+index >= count) //表示没有找到，或者section名字为空
				return false;
			else
			{
				i += index;
				int tempIndex = GetMatchIndex(buf+i+1 , '[', sectionName);
				if (tempIndex <= 0 )
					return false;

				if (!Parse(buf+i+1, tempIndex-1,"\n", "="))
					assert(false);
				AllMap[deleteBlank(sectionName)] = sectionMap;
				
				i += tempIndex;
			}
		}

		return true;
	}

     bool INIParser::Parse(const char* ini_data, size_t ini_data_len, const std::string& line_seperator, const std::string& key_value_seperator)
	{
		if(ini_data == NULL || *ini_data == '\0' || ini_data_len <= 0 ||
				line_seperator.size() == 0 || key_value_seperator.size() == 0)
			return false;
		sectionMap.clear();
		bool valuestart=false;
		std::string key, value;
		size_t i=0;
		
		while(ini_data != NULL && *ini_data != '\0' && i < ini_data_len)
		{
			if(strncmp(ini_data, line_seperator.c_str(), line_seperator.size()) == 0)//到了分隔符位置
			{
				if (!(key.empty()) && !(deleteBlank(key).empty()))//不为空直接加入进去
				{
					std::string tempKey = deleteBlank(key);
					std::string tempValue = deleteBlank(value);
					sectionMap[tempKey] = tempValue;
				}
				i += line_seperator.size();
				key.clear();
				value.clear();
				valuestart = false;
				ini_data += line_seperator.size();
			}
			else if(valuestart)//键值
			{
				++i;
				value += *ini_data;
				++ini_data;
			}
			else
			{
				if (strncmp(ini_data,key_value_seperator.c_str(), key_value_seperator.size()) == 0)//键何值的分隔符
				{
					ini_data += key_value_seperator.size();
					i += key_value_seperator.size();
					valuestart = true;
					continue;
				}
				else//键值
				{
					++i;
					key += *ini_data;
					++ini_data;
				}
			}
		}

		if ( !deleteBlank(key).empty() )//考虑最后没有放进的键值对
		{
			sectionMap[deleteBlank(key)] = deleteBlank(value);
		}

		return true;
	}

	std::string INIParser::deleteBlank(std::string str)
	{
		std::string res;
		for(unsigned i=0;i<str.size();++i)
			if (str[i] != ' ')
				res += str[i];

		return res;
	}

	const std::string& INIParser::Get(const std::string &key, bool *found)
	{
		if (sectionMap.find(key) != sectionMap.end())
		{
		//	*found = true;
			return sectionMap[key];
		}
		else
		{
		//	*found = false;
			return std::string();
		}
	}

	const std::string& INIParser::Get(const std::string &section, const std::string &key, bool *found)
	{
		if (AllMap.find(section) != AllMap.end())
		{
			if (AllMap[section].find(key) != AllMap[section].end())
			{
				// *found = true;
				return AllMap[section][key];
			}	
		}

		return std::string();
	}
	
	int INIParser::GetMatchIndex(const char *data, const char &match_char, std::string &sectionName, bool flag)
	{
		if (data == NULL || *data == '\0')
			return -1;
		int i=0;

		while(data != NULL && *data != '\0' && data[i] != match_char)
		{
			if (flag)
				sectionName += data[i];
			++i;
		}
		if (data != NULL && *data != '\0')
			++i;
		
		if (data == NULL || *data == '\0')
			i = -1;

		return i;
	}
}
