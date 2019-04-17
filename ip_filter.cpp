#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]

using ipvector = std::vector<std::string>;
using ippool = std::vector<ipvector>;

std::vector<std::string> split(const std::string &str, char d)
{
    std::vector<std::string> r;

    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while(stop != std::string::npos)
    {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}

bool compare_ipvectors(ippool::const_iterator left, ippool::const_iterator right)
{
	auto len = left->size()-1;
	ipvector::const_iterator loctet = left->cbegin();
	ipvector::const_iterator roctet = right->cbegin();
	for (auto i=0; i!=len; i++,loctet++,roctet++)
	{
		if(std::lexicographical_compare(loctet->cbegin(), loctet->cend(),
					roctet->cbegin(), roctet->cend()))
		{return true;}
		else if (std::lexicographical_compare(roctet->cbegin(), roctet->cend(),
					loctet->cbegin(), loctet->cend()))
		{return false;}
	}
	return false;
}

void sort_it(bool compare(ippool::const_iterator,ippool::const_iterator),ippool &ips)
{
	auto elems = ips.size();
	for (auto i=0; i<elems; i++)
	{
		for (auto ip = ips.begin(); std::distance(ips.begin(),ip) < elems-i-1; ip++)
		{
			if (compare(ip,ip+1))
			{
				std::swap(*ip,*(ip+1));
			}
		}
	}
}

class IpFilters {
	ippool ips;
	private:
	void print_ips()
	{
		for(auto ip: ips)
		{
			for(auto octet: ip)
			{
				if (octet != *(ip.cbegin()))
					std::cout<<".";
				std::cout << octet;
			}
			std::cout<<std::endl;
		}
	}
	public:
	IpFilters(ippool ips)
	{
		this->ips = ips;
	}

	template<typename... Args>
	ippool filter(Args... args) {
		std::vector <std::tuple <std::string, int> > argstuples;
		auto i=0;
		for (auto arg:{args...})
		{
			argstuples.push_back(std::make_pair(arg, i++));
		}
		for (auto ip: ips)
		{
			for (auto arg: argstuples) {
				if ( arg.first != ip.at[arg.second] )
				{
					ippool::const_iterator it = ips.cbegin();
					it += *(arg.first);
					ips.erase(it);
				}
			}
		}
		print_ips();
	}


};


int main(int argc, char const *argv[])
{
    try
    {
        ippool ip_pool;

        for(std::string line; std::getline(std::cin, line);)
        {
            std::vector<std::string> v = split(line, '\t');
            ip_pool.push_back(split(v.at(0), '.'));
        }

	sort_it((&compare_ipvectors),ip_pool);

        for(ippool::const_iterator ip = ip_pool.cbegin(); ip != ip_pool.cend(); ++ip)
        {
            for(ipvector::const_iterator ip_part = ip->cbegin(); ip_part != ip->cend(); ++ip_part)
            {
                if (ip_part != ip->cbegin())
                {
                    std::cout << ".";

                }
                std::cout << *ip_part;
            }
            std::cout << std::endl;
        }

	IpFilters filters(ip_pool);
	filters.filter(1);


        // 222.173.235.246
        // 222.130.177.64
        // 222.82.198.61
        // ...
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // TODO filter by first byte and output
        // ip = filter(1)

        // 1.231.69.33
        // 1.87.203.225
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // TODO filter by first and second bytes and output
        // ip = filter(46, 70)

        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76

        // TODO filter by any byte and output
        // ip = filter_any(46)

        // 186.204.34.46
        // 186.46.222.194
        // 185.46.87.231
        // 185.46.86.132
        // 185.46.86.131
        // 185.46.86.131
        // 185.46.86.22
        // 185.46.85.204
        // 185.46.85.78
        // 68.46.218.208
        // 46.251.197.23
        // 46.223.254.56
        // 46.223.254.56
        // 46.182.19.219
        // 46.161.63.66
        // 46.161.61.51
        // 46.161.60.92
        // 46.161.60.35
        // 46.161.58.202
        // 46.161.56.241
        // 46.161.56.203
        // 46.161.56.174
        // 46.161.56.106
        // 46.161.56.106
        // 46.101.163.119
        // 46.101.127.145
        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76
        // 46.55.46.98
        // 46.49.43.85
        // 39.46.86.85
        // 5.189.203.46
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
