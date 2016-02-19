// xll.t.cpp - Console testing program for xll12
#include "../xll/xll.h"
#include <ctime>
#include <random>

//[module(name="xll12")];
using namespace xll;

static std::default_random_engine dre;

void test_sref()
{
	REF12 r1, r2;
	ensure (r1 == r2);
	REF12 r3(1,2,3,4);
	REF12 r4(r3);
	ensure (r3 == r4);
	r2 = r3;
	ensure (r2 == r3);

	OPER12 o1(r2);
	ensure (o1.type() == xltypeSRef);
	ensure (r2 == o1.val.sref.ref);

	OPER12 o2(REF12(1,2,3,4));
	ensure (o1 == o2);
	ensure (o2.val.sref.ref == r2);

	{
		REF12 r(1,2,3,4);
		r.move(1,1);
		ensure (r == REF12(2,3,3,4));
		r.up();
		ensure (r == REF12(1,3,3,4));
		r.right();
		ensure (r == REF12(1,4,3,4));
		r.down();
		ensure (r == REF12(2,4,3,4));
		r.left();
		ensure (r == REF12(2,3,3,4));

		ensure (move(r, 2, 3) == REF12(4, 6, 3, 4));
	}
	{
		// strict-weak ordering
		// !(x < y) && !(y < x) is an equivalence relation
		// x R x, x R y => y R x, !(x R y) and y R z => x R z
		std::uniform_int_distribution<RW> u(1, 10);
		auto R = [](const REF12& x, const REF12& y) {
			return !(x < y) && !(y < x);
		};
		auto ref = [&u]() { return REF12(u(dre),u(dre),u(dre),u(dre)); };
		for (int i = 0; i < 100; ++i) {
			auto x = ref(), y = ref(), z = ref();
			ensure (R(x, x));
			ensure (R(y, y));
			ensure (R(z, z));
			ensure (R(x, y) ? R(y, x) : true);
			ensure ((R(x, y) && R(y, z)) ? R(x, z) : true);
		}
	}
}

void test_swap()
{
	OPER12 str(L"foo");
	OPER12 multi(2,3);
	swap(str, multi);
	ensure (str.xltype == xltypeMulti);
	ensure (str.rows() == 2);
	ensure (str.columns() == 3);
	ensure (multi.xltype == xltypeStr);
	ensure (multi == L"foo");
}

void test_oper()
{
	OPER12 o;
	ensure (o.size() == 1);
	ensure (o.xltype == xltypeMissing);
	OPER12 o2(o);
	ensure (o2.xltype == xltypeMissing);
	ensure (o == o2);
	o = o2;
	ensure (o.xltype == xltypeMissing);
	ensure (o == o2);
	ensure (!(o != o2));
}

void test_num()
{
	OPER12 num(1.23);
	ensure (num.xltype == xltypeNum);
	ensure (num.val.num == 1.23);
	ensure (num == 1.23); // operator double
	ensure (1.23 == num);
	ensure (num);
	num = 0.; // calls operator=(double)
	ensure (num.xltype == xltypeNum);
	ensure (num.val.num == 0);
	ensure (num == 0);
	ensure (!num);

	OPER12 num2(num);
	ensure (num == num2);
	num = num2;
	ensure (num == num2);
}

void test_str()
{
//	[cpp_quote(__FUNCDNAME__)];
//	[idl_quote(__FUNCTION__)];
	OPER12 str(L"str");
	ensure (str.xltype == xltypeStr);
	ensure (str.val.str[0] == 3);
	ensure (0 == wcsncmp(str.val.str + 1, L"str", 3));
	ensure (str == L"str");
	OPER12 str2(str);
	ensure (str2.xltype == xltypeStr);
	ensure (str2.val.str[0] == 3);
	ensure (0 == wcsncmp(str2.val.str + 1, L"str", 3));
	ensure (str == str2);
	str = str2;
	ensure (str == str2);

	str = L"foobar";
	ensure (str.xltype == xltypeStr);
	ensure (str == L"foobar");

	OPER12 o;
	o = L"foobaz";
	ensure (o == L"foobaz");
	o &= L"blah";
	ensure (o == L"foobazblah");

	const XCHAR* null = 0;
	OPER12 Null(null);
	ensure (Null.xltype == xltypeMissing);
}
void test_bool()
{
	OPER12 b(true);
	ensure (b.xltype == xltypeBool);
	ensure (b.val.xbool);
	ensure (b.val.xbool == 1);
	ensure (b.val.xbool == TRUE);
}
void test_multi()
{
	OPER12 multi(2,3);
	ensure (multi.xltype == xltypeMulti);
	ensure (multi.rows() == multi.val.array.rows);
	ensure (multi.columns() == multi.val.array.columns);
	ensure (multi[0] == OPER12());

	multi[0] = 1.23;
	multi[1] = L"str";

	int i = 0;
	for (auto& m : multi)
		ensure (m == multi[i++]);

	multi.push_back(multi);
	ensure (multi.xltype == xltypeMulti);
	ensure (multi.rows() == 4);
	ensure (multi.columns() == 3);
/*
	bool thrown = false;
	try {
		multi.push_back(OPER12(1.23));
	}
	catch (...) {
		thrown = true;
	}
	ensure (thrown);
*/
	OPER12 m0;
	m0.push_back(OPER12(L"foo"));
	ensure (m0.xltype == xltypeMulti);
	ensure (m0.rows() == 1);
	ensure (m0.columns() == 2);
	ensure (m0[0].xltype == xltypeMissing);
	ensure (m0[1] == L"foo");

	std::uniform_int_distribution<RW> u(1, 100);
	for (int j = 0; j < 100; ++j) {
		OPER12 a(u(dre), u(dre)), b(u(dre), u(dre));
		if (a.columns() == b.columns()) {
			if (a.rows() == 1 && b.rows() != 1)
				b.resize(1, b.size());
			a.push_back(b);
		}
	}

	{
		OPER12 o{OPER12(1.23), OPER12(L"foo")};
		ensure (o.xltype == xltypeMulti);
		ensure (o.size() == 2);
		ensure (o[0] == 1.23);
		ensure (o[1] == L"foo");
	}
	{
		OPER12 o{{OPER12(1.23)},{OPER12(L"foo"), OPER12(true)}};
		ensure (o.xltype == xltypeMulti);
		ensure (o.rows() == 2);
		ensure (o.columns() == 2);
		ensure (o(0,0) == 1.23);
		ensure (o(0,1) == OPER12());
		ensure (o(1,0) == L"foo");
		ensure (o(1,1) == true);
	}
}
int main()
{
	dre.seed(static_cast<unsigned>(::time(0)));

	test_sref();
	test_swap();
	test_oper();
	test_num();
	test_str();
	test_bool();
	test_multi();

	return 0;
}