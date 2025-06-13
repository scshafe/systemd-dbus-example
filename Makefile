



service:
	g++ service.cpp `pkg-config --cflags --libs dbus-1` `pkg-config --cflags --libs libsystemd` -o scshafe-test-service



client:
	g++ client.cpp `pkg-config --cflags --libs dbus-1` `pkg-config --cflags --libs libsystemd` -o scshafe-test-client
