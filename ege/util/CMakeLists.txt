set(SOURCES
	"Converter.cpp"
	"Converter.h"
	"Equation.cpp"
	"Equation.h"
	"EquationSystem.cpp"
	"EquationSystem.h"
	"JSONConverter.cpp"
	"JSONConverter.h"
	"Math.cpp"
	"Math.h"
	"Object.cpp"
	"Object.h"
	"ObjectArray.cpp"
	"ObjectArray.h"
	"ObjectBoolean.cpp"
	"ObjectBoolean.h"
	"ObjectFloat.cpp"
	"ObjectFloat.h"
	"ObjectInt.cpp"
	"ObjectInt.h"
	"ObjectList.cpp"
	"ObjectList.h"
	"ObjectMap.cpp"
	"ObjectMap.h"
	"ObjectSerializers.cpp"
	"ObjectSerializers.h"
	"ObjectString.cpp"
	"ObjectString.h"
	"ObjectUnsignedInt.cpp"
	"ObjectUnsignedInt.h"
	"PointerUtils.h"
	"Random.cpp"
	"Random.h"
	"Serializable.cpp"
	"Serializable.h"
	"StringUtils.h"
	"Time.cpp"
	"Time.h"
	"Vector.cpp"
	"Vector.h"
	"VectorOperations.cpp"
	"VectorOperations.h"

	"system.h"
	"system.cpp"
	"system/FileSystem.cpp"
	"system/FileSystem.h"
	"system/SystemImpl.cpp"
	"system/SystemImpl.h"
	"system/Time.cpp"
	"system/Time.h"
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux" OR ${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
	set(SOURCES
		${SOURCES}
		"system/SystemImplUnix.cpp"
		"system/SystemImplUnix.h"
	)
endif()

ege_add_module(util)
ege_depend_module(util main)

