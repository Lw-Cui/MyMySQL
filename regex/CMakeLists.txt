SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated")
SET(QueryMySQL Query)
ADD_EXECUTABLE(${QueryMySQL}
	interface.cpp 
	parse.cpp
	build.cpp
)
TARGET_LINK_LIBRARIES(${QueryMySQL} ${QUERY_FUNC})