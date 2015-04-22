#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Inv3D/VoxelFld/Point3D.h"
#include "../Inv3D/VoxelFld/Point3D.cpp"
#include "../Inv3D/VoxelFld/Facet3Pt.h"
#include "../Inv3D/VoxelFld/Facet3Pt.cpp"
#include "../Inv3D/VoxelFld/VoxelFld.h"
#include "../Inv3D/VoxelFld/VoxelFld.cpp"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Inv3D_UnitTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod_Inv3D_VoxelFld)
		{
			CVoxelFld vf;
			vf.Init(10., 10., 10.);

			double g=0.;
			vf.FldGrav(0., 0., 1., g);
			Assert::AreEqual(1.4006803955715917e-009, g, 1.0e-20);

		}

	};
}