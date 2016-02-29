#include <cstdio>
#include <cstdint>
#include <typeinfo>
#include <type_traits>

class IFoo
{
public:
	virtual void Bar() = 0;
	virtual ~IFoo() {}
};

using FooBar = std::integral_constant<decltype(&IFoo::Bar), &IFoo::Bar>;

int main()
{
	std::printf("%s\n", typeid(FooBar).name());
	auto ptr = &IFoo::Bar;
	for (size_t i = 0; i < sizeof(ptr); ++i)
		std::printf("%02x", *((const uint8_t*)&ptr + i));
	std::printf("\n");
	return 0;
}
