project( hnor )
cmake_minimum_required( VERSION 3.16.0 )

file(GLOB hnor_SRC CONFIGURE_DEPENDS
	"src/*.h"
	"src/*.cpp"
)

add_executable( hnor ${hnor_SRC} )
install( TARGETS hnor DESTINATION bin )
install( FILES "hgeoHNOR2020.geoid" DESTINATION bin )