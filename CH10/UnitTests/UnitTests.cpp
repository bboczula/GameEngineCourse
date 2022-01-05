#include "pch.h"
#include "CppUnitTest.h"
#include "../SapphireMath/SapphireMath.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(UnitTests)
	{
	public:
		
		TEST_METHOD(Point3DInitialize01)
		{
			Sapphire::Math::Point3D a{ 1.0f, 2.0f, 3.0f };

			Assert::AreEqual(1.0f, a.x);
			Assert::AreEqual(2.0f, a.y);
			Assert::AreEqual(3.0f, a.z);
		}

		TEST_METHOD(Point3DInitialize02)
		{
			Sapphire::Math::Point3D a;

			Assert::AreEqual(0.0f, a.x);
			Assert::AreEqual(0.0f, a.y);
			Assert::AreEqual(0.0f, a.z);
		}
	};
}
