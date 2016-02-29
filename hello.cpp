#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <utility>

static void Un(int argc, char* argv[])
{
	if (argc != 1)
		return;
	auto a = std::strtoul(argv[0], nullptr, 0);
	unsigned long r;
	__asm__ ("sex.b %0,%1" : "=r" (r) : "r" (a));
	std::printf("sex.b %lu => %lu\n", a, r);
	__asm__ ("sex.s %0,%1" : "=r" (r) : "r" (a));
	std::printf("sex.s %lu => %lu\n", a, r);
	__asm__ ("neg %0,%1" : "=r" (r) : "r" (a));
	std::printf("neg %lu => %lu\n", a, r);
	__asm__ ("not %0,%1" : "=r" (r) : "r" (a));
	std::printf("not %lu => %lu\n", a, r);
}

static void BinU(int argc, char* argv[])
{
	if (argc != 2)
		return;
	auto a = std::strtoul(argv[0], nullptr, 0);
	auto b = std::strtoul(argv[1], nullptr, 0);
	unsigned long r;

	r = a;
	__asm__ ("add %0, %1" : "+r" (r) : "r" (b));
	printf("add %lu %lu => %lu\n", a, b, r);

	r = a;
	__asm__ ("sub %0, %1" : "+r" (r) : "r" (b));
	printf("sub %lu %lu => %lu\n", a, b, r);

	r = a;
	__asm__ ("mul %0, %1" : "+r" (r) : "r" (b));
	printf("mul %lu %lu => %lu\n", a, b, r);
}

static void BinI(int argc, char* argv[])
{
	if (argc != 2)
		return;
	auto a = std::strtol(argv[0], nullptr, 0);
	auto b = std::strtol(argv[1], nullptr, 0);
	long r;

	r = a;
	__asm__ ("add %0, %1" : "+r" (r) : "r" (b));
	printf("add %ld %ld => %ld\n", a, b, r);

	r = a;
	__asm__ ("sub %0, %1" : "+r" (r) : "r" (b));
	printf("sub %ld %ld => %ld\n", a, b, r);
}

#define CMPBCC(s, cc) \
	{ \
		bool taken = false; \
		__asm__ goto ("cmp %0, %1\n\tb" #cc " %2" : : "r" (a), "r" (b) : "cc" : L##cc); \
		if (false) { \
	L##cc: \
			taken = true; \
		} \
		printf("cmp %l" #s " %l" #s ": b" #cc " %s\n", a, b, taken ? "taken" : "not taken"); \
	}

static void CmpU(int argc, char* argv[])
{
	if (argc != 2)
		return;
	auto a = std::strtoul(argv[0], nullptr, 0);
	auto b = std::strtoul(argv[1], nullptr, 0);
	CMPBCC(u, eq)
	CMPBCC(u, ne)
	CMPBCC(u, lt)
	CMPBCC(u, gt)
	CMPBCC(u, ltu)
	CMPBCC(u, gtu)
	CMPBCC(u, ge)
	CMPBCC(u, le)
	CMPBCC(u, geu)
	CMPBCC(u, leu)
}

static void CmpI(int argc, char* argv[])
{
	if (argc != 2)
		return;
	auto a = std::strtol(argv[0], nullptr, 0);
	auto b = std::strtol(argv[1], nullptr, 0);
	CMPBCC(d, eq)
	CMPBCC(d, ne)
	CMPBCC(d, lt)
	CMPBCC(d, gt)
	CMPBCC(d, ltu)
	CMPBCC(d, gtu)
	CMPBCC(d, ge)
	CMPBCC(d, le)
	CMPBCC(d, geu)
	CMPBCC(d, leu)
}

static void DivModU(int argc, char* argv[])
{
	if (argc != 2)
		return;
	auto a = std::strtoul(argv[0], nullptr, 0);
	auto b = std::strtoul(argv[1], nullptr, 0);
	auto rdiv = a;
	__asm__ ("udiv %0, %1" : "+r" (rdiv) : "r" (b));
	std::printf("udiv %lu %lu => %lu\n", a, b, rdiv);
	auto rmod = a;
	__asm__ ("umod %0, %1" : "+r" (rmod) : "r" (b));
	std::printf("umod %lu %lu => %lu\n", a, b, rmod);
}

static void DivModI(int argc, char* argv[])
{
	if (argc != 2)
		return;
	auto a = std::strtol(argv[0], nullptr, 0);
	auto b = std::strtol(argv[1], nullptr, 0);
	auto rdiv = a;
	__asm__ ("div %0, %1" : "+r" (rdiv) : "r" (b));
	std::printf("div %ld %ld => %ld\n", a, b, rdiv);
	auto rmod = a;
	__asm__ ("mod %0, %1" : "+r" (rmod) : "r" (b));
	std::printf("mod %ld %ld => %ld\n", a, b, rmod);
}

static void Sh(int argc, char* argv[])
{
	if (argc != 2)
		return;
	auto a = std::strtoul(argv[0], nullptr, 0);
	auto b = std::strtoul(argv[1], nullptr, 0);
	auto rlshr = a;
	__asm__ ("lshr %0, %1" : "+r" (rlshr) : "r" (b));
	std::printf("lshr %lu %lu => %lu\n", a, b, rlshr);
	auto rashl = a;
	__asm__ ("ashl %0, %1" : "+r" (rashl) : "r" (b));
	std::printf("ashl %lu %lu => %lu\n", a, b, rashl);
	auto rashr = a;
	__asm__ ("ashr %0, %1" : "+r" (rashr) : "r" (b));
	std::printf("ashr %lu %lu => %lu\n", a, b, rashr);
}

static void Convert16to32(int argc, char* argv[])
{
	if (argc != 1)
		return;
	auto c0 = std::strtoul(argv[0], nullptr, 0) & 0xffff;
	auto r = 255 * (c0 >> 11) / 0x1f;
	auto g = 255 * ((c0 >> 5) & 0x3f) / 0x3f;
	auto b = 255 * (c0 & 0x1f) / 0x1f;
	auto c1 = (r << 16) | (g << 8) | b;
	std::printf("rgb16 %#lx => rgb32 %#lx\n", c0, c1);
}

static void Convert32to16(int argc, char* argv[])
{
	if (argc != 1)
		return;
	auto c0 = std::strtoul(argv[0], nullptr, 0) & 0xffffff;
	auto r = 0x1f * (c0 >> 16) / 0xff;
	auto g = 0x3f * ((c0 >> 8) & 0xff) / 0xff;
	auto b = 0x1f * (c0 & 0xff) / 0xff;
	auto c1 = (r << 11) | (g << 5) | b;
	std::printf("rgb32 %#lx => rgb16 %#lx\n", c0, c1);
}

static const std::pair<const char*, void (*)(int, char*[])> g_pairs[] =
{
	{ "un", &Un },
	{ "bin.u", &BinU },
	{ "bin.i", &BinI },
	{ "cmp.u", &CmpU },
	{ "cmp.i", &CmpI },
	{ "divmod.u", &DivModU },
	{ "divmod.i", &DivModI },
	{ "sh", &Sh },
	{ "16to32", &Convert16to32 },
	{ "32to16", &Convert32to16 }
};

int main(int argc, char* argv[])
{
	if (argc <= 1) {
		std::printf("Hello, world!\n");
		return 0;
	}
	for (auto& p : g_pairs) {
		if (std::strcmp(p.first, argv[1]) == 0) {
			p.second(argc - 2, &argv[2]);
			break;
		}
	}
	return 0;
}
