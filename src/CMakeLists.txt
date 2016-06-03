SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated")
SET(QueryMySQL Query)
ADD_EXECUTABLE(${QueryMySQL}
	Query.cpp 
	interface.cpp 
	parse.cpp
)
TARGET_LINK_LIBRARIES(${QueryMySQL} mysqlpp mysqlclient)