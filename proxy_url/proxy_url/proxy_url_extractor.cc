
#include "proxy_url_extractor.h"
#include <fstream>
#include <vector>
#include "tokener.h"
#include <iostream>

namespace qh
{

    namespace {

        template< class _StringVector, 
        class StringType,
        class _DelimType> 
            inline void StringSplit(  
            const StringType& str, 
            const _DelimType& delims, 
            unsigned int maxSplits, 
            _StringVector& ret)
        {
            unsigned int numSplits = 0;

            // Use STL methods
            size_t start, pos;
            start = 0;

            do
            {
                pos = str.find_first_of( delims, start );

                if ( pos == start )
                {
                    ret.push_back(StringType());
                    start = pos + 1;
                }
                else if ( pos == StringType::npos || ( maxSplits && numSplits + 1== maxSplits ) )
                {
                    // Copy the rest of the string
                    ret.push_back(StringType());
                    *(ret.rbegin()) = StringType(str.data() + start, str.size() - start);
                    break;
                }
                else
                {
                    // Copy up to delimiter
                    //ret.push_back( str.substr( start, pos - start ) );
                    ret.push_back(StringType());
                    *(ret.rbegin()) = StringType(str.data() + start, pos - start);
                    start = pos + 1;
                }

                ++numSplits;

            }
            while ( pos != StringType::npos );
        }
    }

    ProxyURLExtractor::ProxyURLExtractor()
    {
    }

    bool ProxyURLExtractor::Initialize( const std::string& param_keys_path )
    {
        std::ifstream ifs;
        ifs.open(param_keys_path.data(), std::fstream::in);
        typedef std::vector<std::string> stringvector;
        stringvector keysvect;
        
        while (!ifs.eof()) {
            std::string line;
            getline(ifs, line);
            if (ifs.fail() && !ifs.eof()) {
                fprintf(stderr, "SubUrlExtractor::LoadParamKeysFile readfile_error=[%s] error!!", param_keys_path.data());
                ifs.close();
                return false;
            }
            if (line.empty()) continue;

            keysvect.clear();
            StringSplit(line, ",", static_cast<unsigned int>(-1), keysvect);
            assert(keysvect.size() >= 1);
            keys_set_.insert(keysvect.begin(), keysvect.end());
            keys_set_.erase("");
        }

        ifs.close();

        return true;
    }

    std::string ProxyURLExtractor::Extract( const std::string& raw_url )
    {
        std::string sub_url;
        ProxyURLExtractor::Extract(keys_set_, raw_url, sub_url);
        return sub_url;
    }

	int ProxyURLExtractor::getMatchIndexIncrement(const std::string &keys,const std::string &raw_url,int lastIndex)
	{
		int keySize = keys.size();
		for(int i=keySize-1; i >= 0; --i)
		{
			if(keys[i] != raw_url[lastIndex])//字符不匹配
			{
				if (keys.find_last_of(raw_url[lastIndex]) == std::string::npos)
					return i+1;
				else
					return i - keys.find_last_of(raw_url[lastIndex]);
			}
			else
				--lastIndex;
		}

		return 0; //匹配的标志
	}

	int ProxyURLExtractor::getIndex(const std::string &keys,const std::string &raw_url)
	{
		if (raw_url.empty() || keys.empty())
			return -1;
		int keySize = keys.size();
		int strSize = raw_url.size();
		for(int i=keySize-1; i < strSize;)
		{
			int index = getMatchIndexIncrement(keys,raw_url,i);
			if( index == 0 )
				return i;
			else
				i += index;
		}

		return -1; //没有找到
	}

    void ProxyURLExtractor::Extract( const KeyItems& keys, const std::string& raw_url, std::string& sub_url )
    {
#if 1
        //TODO 请面试者在这里添加自己的代码实现以完成所需功能
		if (keys.empty() || raw_url.empty())
			return;

		for(std::set<std::string>::iterator itr=keys.begin(); itr != keys.end(); ++itr)
		{
			std::string tempKey = *itr;
			int pos = getIndex(tempKey+'=',raw_url);
			if (pos == -1)//没有找到匹配的子字符，继续查找下一个关键字符串
				continue;
			else
			{
				if (raw_url[pos-1-tempKey.size()] == '?' || raw_url[pos-1-tempKey.size()] == '&')//检测是否是关键字符串
				{
					++pos;
					while(raw_url[pos] != '\0' && raw_url[pos] != '&')
						sub_url += raw_url[pos++];
					return;
				}
			}
		}
	
#else
        //这是一份参考实现，但在特殊情况下工作不能符合预期
        Tokener token(raw_url);
        token.skipTo('?');
        token.next(); //skip one char : '?' 
        std::string key;
        while (!token.isEnd()) {
            key = token.nextString('=');
            if (keys.find(key) != keys.end()) {
                const char* curpos = token.getCurReadPos();
                int nreadable = token.getReadableSize();

                /**
                * case 1: 
                *  raw_url="http://www.microsofttranslator.com/bv.aspx?from=&to=zh-chs&a=http://hnujug.com/&xx=yy"
                *  sub_url="http://hnujug.com/"
                */
                sub_url = token.nextString('&');

                if (sub_url.empty() && nreadable > 0) {
                    /**
                    * case 2: 
                    * raw_url="http://www.microsofttranslator.com/bv.aspx?from=&to=zh-chs&a=http://hnujug.com/"
                    * sub_url="http://hnujug.com/"
                    */
                    assert(curpos);
                    sub_url.assign(curpos, nreadable);
                }
            }
            token.skipTo('&');
            token.next();//skip one char : '&'
        }
#endif
    }

    std::string ProxyURLExtractor::Extract( const KeyItems& keys, const std::string& raw_url )
    {
        std::string sub_url;
        ProxyURLExtractor::Extract(keys, raw_url, sub_url);
        return sub_url;
    }
}

