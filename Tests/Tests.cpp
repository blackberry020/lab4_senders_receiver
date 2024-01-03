#include "pch.h"
#include "CppUnitTest.h"

#include "..\func.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
	TEST_CLASS(Tests)
	{
	public:
		
		TEST_METHOD(TestIsNumber)
		{
			Assert::IsTrue(isNumber("123"));
		}

		TEST_METHOD(TestIsNumberNotNumber)
		{
			Assert::IsFalse(isNumber("123qwerty"));
		}

		TEST_METHOD(TestIsNumberSpace)
		{
			Assert::IsFalse(isNumber("123 "));
		}

		TEST_METHOD(TestIsNumberEmpty)
		{
			Assert::IsFalse(isNumber(""));
		}

		TEST_METHOD(TestIsNumberLeadingZero)
		{
			Assert::IsTrue(isNumber("0123"));
		}
	};
}
