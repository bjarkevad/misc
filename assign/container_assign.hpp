template<typename T> class ContainerAssigner
{
	private:
		T& vp;
	public:
		ContainerAssigner(const ContainerAssigner<T>&a)
			: vp(a.vp) {};
		ContainerAssigner(T& v) : vp(v) {};
		ContainerAssigner<T>& operator,(const typename T::value_type& i)
		{
			vp.insert(vp.end(), i);
			return *this;
		};
};

template<typename T> ContainerAssigner<T> operator+=(T& v, const typename T::value_type& i)
{
	v.insert(v.end(), i);
	return ContainerAssigner<T>(v);
}
