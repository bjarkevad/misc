#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

template<typename T, typename Ret> struct has_value_type
{
	typedef typename T::value_type value_type;
	typedef Ret return_type;
};

template<typename T> typename has_value_type<T,std::ostream&>::return_type operator<<(std::ostream& ost, const T& vec)
{
	ost.operator<< "[";
	std::copy(vec.begin(), vec.end()-1, std::ostream_iterator<typename T::value_type>(ost, ", "));
	ost.operator<< *(vec.end()-1);
	ost.operator<< "]";
	return ost;
}

int main()
{
	std::vector<int> x;
	x.push_back(1); x.push_back(2); x.push_back(3);

	std::vector<std::string> y;
	y.push_back("abc"); y.push_back("def"); y.push_back("ghi");

	std::cout << x << std::endl;
	std::cout << y << std::endl;

	return 0;
}
