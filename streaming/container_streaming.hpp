#include <algorithm>
#include <iostream>

template<typename T, typename Ret> struct Iterable
{
		typedef typename T::const_iterator iter_type;
		typedef Ret return_type;
};

template<typename T> class Printer
{
	public:
		Printer(std::ostream& ost) : out(ost), first(true) {};
		void operator()(const T& item)
		{
			if ( ! first )
			{
				first = false;
				out.operator<<(", ");
			}
			out.operator<<(item);
		};
	private:
		std::ostream& out;
		bool first;
};

template<typename T> typename Iterable<T, std::ostream&>::return_type operator<<(std::ostream& ost, T& container)
{
	Printer<typename T::value_type> p(ost);
	ost.operator<<("[ ");
	for_each(container.begin(), container.end(), p);
	ost.operator<<(" ]");
	return ost;
}
