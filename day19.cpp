#include <cstring>
#include <cstdlib>

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

using namespace std;
typedef std::pair<std::string, unsigned> cached_key;

struct cached_hash
{
	std::size_t operator()(const cached_key& key) const
	{
		return std::hash<std::string>{}(key.first) ^
			std::hash<unsigned>{}(key.second);
	}
};
std::unordered_map<cached_key, bool, cached_hash> cache;

struct rule
{
	virtual bool matched(const std::string& s) const { return false; }
	virtual ~rule();
};
rule::~rule() {}
std::unique_ptr<rule> rules[256];

struct constant : rule
{
	constant(const std::string& s) : str(s) {}
	bool matched(const string& s) const
	{
		return str == s;
	}
	std::string str;
};

struct simplerule : rule
{
	simplerule(unsigned rulenum)
		: rulenum(rulenum) {}
	bool matched(const std::string& s) const
	{
		bool ret = rules[rulenum]->matched(s);
		return ret;
	}
	unsigned rulenum;
};


struct listrule : rule
{
	listrule(std::unique_ptr<rule> first,
		 std::unique_ptr<rule> second)
		: first(std::move(first)), second(std::move(second))
	{ }
	bool matched(const std::string& s) const
	{
		for (size_t i = 1; i < s.size(); i++) {
			std::string s1 = s.substr(0, i);
			std::string s2 = s.substr(i);
			if (first->matched(s1) && second->matched(s2))
				return true;
		}
		return false;
	}
	std::unique_ptr<rule> first;
	std::unique_ptr<rule> second;
};

struct altrule : rule
{
	altrule(std::unique_ptr<rule> first,
		std::unique_ptr<rule> second)
		: first(std::move(first)), second(std::move(second))
	{ }
	bool matched(const std::string& s) const
	{
		if (first->matched(s)) {
			return true;
		} else if (second->matched(s)) {
			return true;
		}
		return false;
	}
	std::unique_ptr<rule> first;
	std::unique_ptr<rule> second;
};

struct cachedrule : rule
{
	cachedrule(unsigned rulenum, std::unique_ptr<rule> therule)
		: rulenum(rulenum), therule(std::move(therule)) {}
	bool matched(const std::string& s) const
	{
		cached_key key(s, rulenum);
		auto cached = cache.find(key);
		if (cached != cache.end())
			return cached->second;
		bool ret = therule->matched(s);
		cache[key] = ret;
		return ret;
	}
	unsigned rulenum;
	std::unique_ptr<rule> therule;
};

#define BUFSIZE 4096

std::unique_ptr<rule> parsenoalt(char *p) {
	while (*p && (*p < '0' || '9' < *p))
		p++;
	if (!*p)
		return nullptr;
	std::unique_ptr<rule> first(new simplerule(strtoul(p, &p, 10)));
	std::unique_ptr<rule> second = parsenoalt(p);

	if (!second)
		return first;
	return std::unique_ptr<rule>(
		new listrule(std::move(first), std::move(second)));
}

std::unique_ptr<rule> parsealt(char *p, char *q) {
	*q = 0;
	std::unique_ptr<rule> first = parsenoalt(p);
	std::unique_ptr<rule> second;
	std::unique_ptr<rule> result;
	q++;
	if ((p = strchr(q, '|')))
		second = std::move(parsealt(q, p));
	else
		second = std::move(parsenoalt(q));
	result.reset(new altrule(std::move(first), std::move(second)));
	return result;
}

void makerules(std::string& line)
{
	char *p, *q;
	unsigned rulenum = strtoul(line.c_str(), &p, 10);
	p++;
	if ((q = strchr(p, '"'))) {
		q++;
		*strchr(q, '"') = 0;
		rules[rulenum].reset(new constant(q));
	} else if ((q = strchr(p, '|'))) {
		rules[rulenum].reset(
			new cachedrule(rulenum, parsealt(p, q)));
	} else {
		rules[rulenum].reset(
			new cachedrule(rulenum, parsenoalt(p)));
	}
}

int main() {
	std::string line;
	while (std::getline(std::cin, line) && !line.empty()) {
		makerules(line);
	}
	line = "8: 42 | 42 8";
	makerules(line);
	line = "11: 42 31 | 42 11 31";
	makerules(line);

	unsigned count = 0;
	while (std::getline(std::cin, line) && !line.empty())
		if (rules[0]->matched(line)) {
			count++;
		}

	std::cout << count << '\n';

	return 0;
}
