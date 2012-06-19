#include <string>
#include <vector>

template<typename T> class Assigner
{
	private:
		std::vector<T>& vp;
	public:
		Assigner(const Assigner<T>& a) : vp(a.vp) {};
		Assigner(std::vector<T>& v) : vp(v) {};
		Assigner<T>& operator,(const T& i)
		{
			vp.push_back(i);
			return *this;
		};
};

template<typename T> Assigner<T> operator+=(std::vector<T>& v, const T& i)
{
	v.push_back(i);
	return Assigner<T>(v);
}

// Specialisations for strings
template<> class Assigner<std::string>
{
	private:
		std::vector<std::string>& vp;
	public:
		Assigner(const Assigner<std::string>& a) : vp(a.vp) {};
		Assigner(std::vector<std::string>& v) : vp(v) {};
		Assigner<std::string>& operator,(const char* i)
		{
			vp.push_back(std::string(i));
			return *this;
		};
};

Assigner<std::string> operator+=(std::vector<std::string>& v, const char* i)
{
	v.push_back(std::string(i));
	return Assigner<std::string>(v);
}

